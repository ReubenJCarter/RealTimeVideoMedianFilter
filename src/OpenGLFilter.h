#pragma once

#include "CommonHeader.h"

class OpenGLFilter
{
	public:		
		GLFWwindow* window;
		GLuint shader_programme;
		GLuint vbo;
		GLuint vao;
		GLuint texture;
		
		int width;
		int height; 
		
		OpenGLFilter();		
		bool Init(int W, int H, std::string name, bool fitlerOn=false);
		void CopyImageData(void* data, int W, int H);
		void MakeCurrent();
		void Update();
		void Stop();
		bool QuitPressed();
};