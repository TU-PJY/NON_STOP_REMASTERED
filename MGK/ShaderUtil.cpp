#include "ShaderUtil.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <fstream>

GLuint ImageShader;
GLuint TextShader;

ShaderLocation ImageTransparencyLocation, ImageColorLocation, ImageModelLocation;
ShaderLocation BlurStrengthLocation, BoolBlurLocation, TexelSizeLocation;
ShaderLocation TextTransparencyLocation, TextColorLocation, TextModelLocation;
ShaderLocation ImageProjectionLocation, ImageViewLocation, ImageViewPositionLocation;
ShaderLocation TextProjectionLocation, TextViewLocation, TextViewPositionLocation;

char* ShaderUtil::LoadShaderFile(const char* FileName) {
	std::ifstream ShaderFile(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ShaderFile.is_open()) {
		std::cout << "Can not open shader file: " << FileName << std::endl;
		exit(EXIT_FAILURE);
	}

	std::streamsize Length = ShaderFile.tellg();
	ShaderFile.seekg(0, std::ios::beg);

	char* Buffer = new char[Length + 1];

	if (!ShaderFile.read(Buffer, Length)) {
		delete[] Buffer;
		ShaderFile.close();
		std::cout << "Can not read shader file: " << FileName << std::endl;
		exit(EXIT_FAILURE);
	}

	Buffer[Length] = '\0';

	ShaderFile.close();

	return Buffer;
}

void ShaderUtil::LoadVertexShader(const char* VertexShader) {
	vertex_source = LoadShaderFile(VertexShader);
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);
	glCompileShader(vertex_shader);

	GLint Result{};
	GLchar ErrorLog[512]{};

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, ErrorLog);
		std::cout << "ERROR: vertex shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void ShaderUtil::LoadFragmentShader(const char* FragmentShader) {
	fragment_source = LoadShaderFile(FragmentShader);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);
	glCompileShader(fragment_shader);

	GLint Result{};
	GLchar ErrorLog[512]{};

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, ErrorLog);
		std::cout << "ERROR: fragment shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void ShaderUtil::CreateShader(GLuint& Shader) {
	Shader = glCreateProgram();
	glAttachShader(Shader, vertex_shader);
	glAttachShader(Shader, fragment_shader);

	glLinkProgram(Shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(Shader);
}

void ShaderUtil::CreateShaderLocation() {
	// Image Shader
	ImageTransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	ImageColorLocation =		glGetUniformLocation(ImageShader, "objectColor");
	BlurStrengthLocation =		glGetUniformLocation(ImageShader, "Radius");
	BoolBlurLocation =			glGetUniformLocation(ImageShader, "UseBlur");
	TexelSizeLocation =			glGetUniformLocation(ImageShader, "TexelSize");
	ImageModelLocation =		glGetUniformLocation(ImageShader, "model");

	// Text Shader
	TextTransparencyLocation =  glGetUniformLocation(TextShader, "transparency");
	TextColorLocation =			glGetUniformLocation(TextShader, "objectColor");
	TextModelLocation =			glGetUniformLocation(TextShader, "model");

	// Image Camera
	ImageProjectionLocation =	glGetUniformLocation(ImageShader, "projection");
	ImageViewLocation =			glGetUniformLocation(ImageShader, "view");
	ImageViewPositionLocation = glGetUniformLocation(ImageShader, "viewPos");
	
	// Text Camera
	TextProjectionLocation =	glGetUniformLocation(TextShader, "projection");
	TextViewLocation =		    glGetUniformLocation(TextShader, "view");
	TextViewPositionLocation =  glGetUniformLocation(TextShader, "viewPos");
}