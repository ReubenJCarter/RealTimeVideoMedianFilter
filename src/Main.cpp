#include "CommonHeader.h"
#include "CLHelper.h"
#include "OpenGLFilter.h"

unsigned int g_seed = 20324;

inline int fast_rand(void) 
{
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}


int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cerr << "not enough args video file" << std::endl;
		return 1; 
	}
	
	float noiseProb = 0.1f;
	
	
	
	
	//init ffmpeg
	av_register_all();
	AVFormatContext *pFormatCtx = NULL;
	if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0)
	{
		std::cerr << "avformat_open_input error" << std::endl; 
		return 1;
	}
	
	if(avformat_find_stream_info(pFormatCtx, NULL)<0)
	{
		std::cerr << "avformat_find_stream_info error" << std::endl; 
		return 1;
	}
	
	av_dump_format(pFormatCtx, 0, argv[1], 0);
	
	int i;
	AVCodecContext *pCodecCtx2 = NULL;
	AVCodecContext *pCodecCtx = NULL;

	// Find the first video stream
	int videoStream=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++)
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) 
		{
			videoStream=i;
			break;
		}
	if(videoStream==-1)
	{
		std::cerr << "could not find any video stream in file" << std::endl; 
		return 1; // Didn't find a video stream
	}

	// Get a pointer to the codec context for the video stream
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	

	AVCodec *pCodec = NULL;

	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec == NULL) 
	{
		std::cerr << "unsupported codec" << std::endl; 
		return 1; // Codec not found
	}
	
	// Open codec
	if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
	{
		std::cerr << "Couldn't open codec" << std::endl;
	  return 1; // Could not open codec
	}
	
	// Allocate video frame
	AVFrame *pFrame = av_frame_alloc();

	// Allocate an AVFrame structure
	AVFrame* pFrameRGB = av_frame_alloc();
	if(pFrameRGB == NULL)
	{
		std::cerr << "av_frame_alloc Couldn't aloocate mmem" << std::endl;
		return 1;
	}
	
	uint8_t *buffer = NULL;
	int numBytes;
	// Determine required buffer size and allocate buffer
	numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	
	
	
	struct SwsContext *sws_ctx = NULL;
	int frameFinished;
	AVPacket packet;
	// initialize SWS context for software scaling
	sws_ctx = sws_getContext(pCodecCtx->width,
		pCodecCtx->height,
		pCodecCtx->pix_fmt,
		pCodecCtx->width,
		pCodecCtx->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
		);

	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
		 pCodecCtx->width, pCodecCtx->height);
		
	i = 0;
	
	int W = pCodecCtx->width;
	int H = pCodecCtx->height;
	OpenGLFilter glFilter;
	glFilter.Init(W, H, "filter", true);
	
	OpenGLFilter glNoFilter;
	glNoFilter.Init(W, H, "no filter");
	
	
	while(av_read_frame(pFormatCtx, &packet) >= 0) 
	{
	  // Is this a packet from the video stream?
	  if(packet.stream_index == videoStream) 
	  {
		// Decode video frame
		avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
		
		// Did we get a video frame?
		if(frameFinished) 
		{
			// Convert the image from its native format to RGB
			sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
					  pFrame->linesize, 0, pCodecCtx->height,
					  pFrameRGB->data, pFrameRGB->linesize);
					 
			
			
			for(int i = 0; i < numBytes; i++)
			{
				float number = (float)fast_rand() / 32767.0f;
				
				if(number < noiseProb/2)
					buffer[i] = 0; 
				else if(number < noiseProb)
					buffer[i] = 255; 
			}
			
			glFilter.MakeCurrent();
			glFilter.CopyImageData(pFrameRGB->data[0], W, H);
			glFilter.Update();
			if(glFilter.QuitPressed()) break; 
			
			glNoFilter.MakeCurrent();
			glNoFilter.CopyImageData(pFrameRGB->data[0], W, H);
			glNoFilter.Update();
			if(glNoFilter.QuitPressed()) break; 
		}
	  }
		
	  // Free the packet that was allocated by av_read_frame
	  av_free_packet(&packet);
	}	
	
	glFilter.MakeCurrent();
	glFilter.Stop();
	
	glNoFilter.MakeCurrent();
	glNoFilter.Stop();
	
	
	
	return 1;
}