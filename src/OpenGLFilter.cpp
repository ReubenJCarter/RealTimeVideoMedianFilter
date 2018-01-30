#include "OpenGLFilter.h"


OpenGLFilter::OpenGLFilter()
{
}


bool OpenGLFilter::Init(int W, int H, std::string name, bool fitlerOn)
{
    /* Initialize the library */
    if (!glfwInit())
	{
		return false;
	}
	
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(W, H, name.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	
	glewInit();

	
	const char* vertex_shader = R"(
	#version 140
	in vec3 vp;
	out vec2 texco;
	void main() 
	{
	  gl_Position = vec4(vp, 1.0);
	  texco = vec2(vp.x, vp.y) * 0.5f + vec2(0.5f, 0.5f);
	}
	
	)";
	
	const char* fragment_shader_nofilter = R"(
	#version 140
	in vec2 texco;
	uniform sampler2D tex;
	out vec4 frag_colour;
	void main()
	{
		frag_colour = texture(tex, texco);
	}
	
	)";
	
	const char* fragment_shader = R"(
	#version 140
	#define S(a,b) tmpb = b; tmpa = a; a = min(tmpa,tmpb); b = max(tmpa,tmpb);
	#define S25(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);S(r,s);S(s,t);S(t,u);S(u,v);S(v,w);S(w,x);S(x,y);
	#define S24(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);S(r,s);S(s,t);S(t,u);S(u,v);S(v,w);S(w,x);
	#define S23(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);S(r,s);S(s,t);S(t,u);S(u,v);S(v,w);	
	#define S22(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p); S(p,q);S(q,r);S(r,s);S(s,t);S(t,u);S(u,v);		
	#define S21(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);S(r,s);S(s,t);S(t,u);	
	#define S20(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);S(r,s);S(s,t);	
	#define S19(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);S(r,s);
	#define S18(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);S(q,r);	
	#define S17(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);S(p,q);	
	#define S16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);S(o,p);	
	#define S15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);S(n,o);	
	#define S14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);S(m,n);	
	#define S13(a,b,c,d,e,f,g,h,i,j,k,l,m) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);S(l,m);
	#define S12(a,b,c,d,e,f,g,h,i,j,k,l) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);S(k,l);		
	#define S11(a,b,c,d,e,f,g,h,i,j,k) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);S(j,k);
	#define S10(a,b,c,d,e,f,g,h,i,j) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);S(i,j);		
	#define S9(a,b,c,d,e,f,g,h,i) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);S(h,i);
	#define S8(a,b,c,d,e,f,g,h) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);S(g,h);
	#define S7(a,b,c,d,e,f,g) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);S(f,g);
	#define S6(a,b,c,d,e,f) S(a,b);S(b,c);S(c,d);S(d,e);S(e,f);
	#define S5(a,b,c,d,e) S(a,b);S(b,c);S(c,d);S(d,e);
	#define S4(a,b,c,d) S(a,b);S(b,c);S(c,d);
	#define S3(a,b,c) S(a,b);S(b,c);
	#define S2(a,b) S(a,b);
	
	out vec4 glFragColor;
	in vec2 texco;

	const int size = 5;
	uniform sampler2D tex;
	
	uniform int WIDTH;
	uniform int HEIGHT;
	
	float MedianR()
	{
		float A[25];
		
		int sizesize = size * size;
		int halfSizesize = sizesize / 2;
		
		
		int index = 0;
		int i = 0; 
		int j = 0;
		float delXOrig = texco.x - (float(size) * (1.0f / float(WIDTH)) / 2.0) + 0.5f / float(WIDTH);
		float delX = delXOrig;
		float delY = texco.y - (float(size) * (1.0f / float(HEIGHT)) / 2.0) + 0.5f / float(HEIGHT);
		while(i < size)
		{
			j = 0;
			delX = delXOrig;
			while(j < size)
			{
				A[index] = texture(tex , vec2(delX, delY)).r;
				delX += 1.0f / float(WIDTH);
				index++;
				j++;
			}
			delY += 1.0f / float(HEIGHT);
			i++;
		}
		
		float tmpa;float tmpb;
		
		S25(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23], A[24])
		S24(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23])
		S23(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22])
		S22(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21])
		S21(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20])
		S20(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19])
		S19(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18])
		S18(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17])
		S17(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16])
		S16(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15])
		S15(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14])
		S14(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13])
		S13(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12])
		S12(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11])
		S11(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10])
		S10(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9])
		S9(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8])
		S8(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7])
		S7(A[0], A[1], A[2], A[3], A[4], A[5], A[6])
		S6(A[0], A[1], A[2], A[3], A[4], A[5])
		S5(A[0], A[1], A[2], A[3], A[4])
		S4(A[0], A[1], A[2], A[3])
		S3(A[0], A[1], A[2])
		S2(A[0], A[1])
		
		
		return A[halfSizesize];
	}
	
	float MedianG()
	{
		float A[25];
		
		int sizesize = size * size;
		int halfSizesize = sizesize / 2;
		
		
		int index = 0;
		int i = 0; 
		int j = 0;
		float delXOrig = texco.x - (float(size) * (1.0f / float(WIDTH)) / 2.0) + 0.5f / float(WIDTH);
		float delX = delXOrig;
		float delY = texco.y - (float(size) * (1.0f / float(HEIGHT)) / 2.0) + 0.5f / float(HEIGHT);
		while(i < size)
		{
			j = 0;
			delX = delXOrig;
			while(j < size)
			{
				A[index] = texture(tex , vec2(delX, delY)).g;
				delX += 1.0f / float(WIDTH);
				index++;
				j++;
			}
			delY += 1.0f / float(HEIGHT);
			i++;
		}
		
		float tmpa;float tmpb;
		
		S25(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23], A[24])
		S24(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23])
		S23(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22])
		S22(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21])
		S21(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20])
		S20(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19])
		S19(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18])
		S18(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17])
		S17(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16])
		S16(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15])
		S15(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14])
		S14(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13])
		S13(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12])
		S12(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11])
		S11(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10])
		S10(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9])
		S9(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8])
		S8(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7])
		S7(A[0], A[1], A[2], A[3], A[4], A[5], A[6])
		S6(A[0], A[1], A[2], A[3], A[4], A[5])
		S5(A[0], A[1], A[2], A[3], A[4])
		S4(A[0], A[1], A[2], A[3])
		S3(A[0], A[1], A[2])
		S2(A[0], A[1])
		
		
		return A[halfSizesize];
	}
	
	float MedianB()
	{
		float A[25];
		
		int sizesize = size * size;
		int halfSizesize = sizesize / 2;
		
		
		int index = 0;
		int i = 0; 
		int j = 0;
		float delXOrig = texco.x - (float(size) * (1.0f / float(WIDTH)) / 2.0) + 0.5f / float(WIDTH);
		float delX = delXOrig;
		float delY = texco.y - (float(size) * (1.0f / float(HEIGHT)) / 2.0) + 0.5f / float(HEIGHT);
		while(i < size)
		{
			j = 0;
			delX = delXOrig;
			while(j < size)
			{
				A[index] = texture(tex , vec2(delX, delY)).b;
				delX += 1.0f / float(WIDTH);
				index++;
				j++;
			}
			delY += 1.0f / float(HEIGHT);
			i++;
		}
		
		float tmpa;float tmpb;
		
		S25(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23], A[24])
		S24(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23])
		S23(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21], A[22])
		S22(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20], A[21])
		S21(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19], A[20])
		S20(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18], A[19])
		S19(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17], A[18])
		S18(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16], A[17])
		S17(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15], A[16])
		S16(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14], A[15])
		S15(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13], A[14])
		S14(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12], A[13])
		S13(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11], A[12])
		S12(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10], A[11])
		S11(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9], A[10])
		S10(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8], A[9])
		S9(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7], A[8])
		S8(A[0], A[1], A[2], A[3], A[4], A[5], A[6] , A[7])
		S7(A[0], A[1], A[2], A[3], A[4], A[5], A[6])
		S6(A[0], A[1], A[2], A[3], A[4], A[5])
		S5(A[0], A[1], A[2], A[3], A[4])
		S4(A[0], A[1], A[2], A[3])
		S3(A[0], A[1], A[2])
		S2(A[0], A[1])
		
		
		return A[halfSizesize];
	}
	
	void main()
	{
	
		float medR = MedianR();
		float medG = MedianG();
		float medB = MedianB();
		//float medR = texture(tex, texco).x;
		//float medG = texture(tex, texco).y;
		//float medB = texture(tex, texco).z;
		glFragColor = vec4(medR, medG, medB, 1.0);
	})";
	
	
	float points[] = 
	{
		-1.0f,  -1.0f,  0.0f,
		1.0f, -1.0f,  0.0f,
		1.0f, 1.0f,  0.0f,
		1.0f, 1.0f,  0.0f,
		-1.0f, 1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f
	};

	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), points, GL_STATIC_DRAW);
	
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	if(fitlerOn)
		glShaderSource(fs, 1, &fragment_shader, NULL);
	else
		glShaderSource(fs, 1, &fragment_shader_nofilter, NULL);
	glCompileShader(fs);
	
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	
	//check shader compilation for errors
	GLint isCompiled = 0;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fs, maxLength, &maxLength, &errorLog[0]);

		//print error log
		std::cerr << &errorLog[0] << std::endl << std::endl;
	
	}
	
	GLint isLinked = 0;
	glGetProgramiv(shader_programme, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shader_programme, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shader_programme, maxLength, &maxLength, &infoLog[0]);
		
		//We don't need the program anymore.
		glDeleteProgram(shader_programme);
	
		//log the info
		std::cerr << &infoLog[0] << std::endl;
	}
	
	width = W;
	height = H;
	
	return true; 
	
    
}


void OpenGLFilter::MakeCurrent()
{
	glfwMakeContextCurrent(window);
}


void OpenGLFilter::CopyImageData(void* data, int W, int H)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLFilter::Update()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
	
    /* Poll for and process events */
    glfwPollEvents();
	
	glUseProgram(shader_programme);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glUniform1i(glGetUniformLocation(shader_programme, "tex"), 0);
	
	glUniform1i(glGetUniformLocation(shader_programme, "WIDTH"), width);
	glUniform1i(glGetUniformLocation(shader_programme, "HEIGHT"), height);
	
	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void OpenGLFilter::Stop()
{
	glfwTerminate();
}


bool OpenGLFilter::QuitPressed()
{
	return glfwWindowShouldClose(window);
}
