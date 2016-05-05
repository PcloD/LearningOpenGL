#pragma once

#include <GL/glew.h>

class Shader {
	public:
		Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath); //Construct a new shader given the file paths to their respective componenets
		GLuint program; //program ID for this shader
		void use(); //Bind this shader to OpenGL
};