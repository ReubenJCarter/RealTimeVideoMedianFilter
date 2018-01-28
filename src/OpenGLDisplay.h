#pragma once

#include "CommonHeader.h"

class OpenGLDisplay
{
	public:		
		GLFWwindow* window;
		GLuint shader_programme;
		GLuint vbo;
		GLuint vao;
		GLuint texture;
		
		OpenGLDisplay();		
		bool Init(int W, int H, std::string name);
		void CopyImageData(void* data, int W, int H);
		void Update();
		void Stop();
		bool QuitPressed();
};