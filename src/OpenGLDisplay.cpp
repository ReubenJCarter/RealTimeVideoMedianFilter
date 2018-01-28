#include "OpenGLDisplay.h"


OpenGLDisplay::OpenGLDisplay()
{
}


bool OpenGLDisplay::Init(int W, int H, std::string name)
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
	#version 400
	in vec3 vp;
	out vec2 texco;
	void main() 
	{
	  gl_Position = vec4(vp, 1.0);
	  texco = vec2(vp.x, vp.y) * 0.5f + vec2(0.5f, 0.5f);
	}
	
	)";
	
	const char* fragment_shader = R"(
	#version 400
	in vec2 texco;
	uniform sampler2D tex;
	out vec4 frag_colour;
	void main() 
	{
		frag_colour = texture(tex, texco);
	}
	
	)";
	
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
	glShaderSource(fs, 1, &fragment_shader, NULL);
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
	
	
	return true; 
	
    
}


void OpenGLDisplay::CopyImageData(void* data, int W, int H)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLDisplay::Update()
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
	
	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void OpenGLDisplay::Stop()
{
	glfwTerminate();
}


bool OpenGLDisplay::QuitPressed()
{
	return glfwWindowShouldClose(window);
}
