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
		
		OpenGLFilter();		
		bool Init(int W, int H, std::string name);
		void CopyImageData(void* data, int W, int H);
		void Update();
		void Stop();
		bool QuitPressed();
};