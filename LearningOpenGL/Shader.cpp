#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	vertexShaderFile.exceptions(std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::badbit);
	

	//Try to open the files, then parse them into strings
	try {
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();
		
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cerr << "Shader file not read" << std::endl;
	}

	//Now get them as char*
	const GLchar* vertexShaderCode = vertexCode.c_str();
	const GLchar* fragmentShaderCode = fragmentCode.c_str();

	//Compile shaders
	GLint successfulCompilation;
	GLchar infoLog[512];

	//Vertex
	GLuint vertexShaderID;
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL); //Attach shader code
	glCompileShader(vertexShaderID); //Then compile it
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &successfulCompilation);
	if (!successfulCompilation) { //If we didn't sucessfully compile, spit out some error code
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cerr << "Vertex shader compilation failed.\n" << infoLog << std::endl;
	}

	//Fragment
	GLuint fragmentShaderID;
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL); //Attach shader code
	glCompileShader(fragmentShaderID); //Then compile it
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &successfulCompilation);
	if (!successfulCompilation) { //If we didn't sucessfully compile, spit out some error code
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cerr << "Fragment shader compilation failed.\n" << infoLog << std::endl;
	}

	//Finally, link the shader programs
	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShaderID); //Attach both
	glAttachShader(this->program, fragmentShaderID);
	glLinkProgram(this->program); //Link
	
	//Print linking errors
	glGetProgramiv(this->program, GL_LINK_STATUS, &successfulCompilation);
	if (!successfulCompilation) {
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed.\n" << infoLog << std::endl;
	}

	//Now cleanup
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::use() { //Tell OpenGL to use this shader
	glUseProgram(this->program);
}