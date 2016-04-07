#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>

namespace ShaderManager {
	std::string loadFile(const char* file); //Returns string containing contents of given file
	GLuint compileShader(GLenum shaderType, std::string shaderCode); //Compiles vertex or fragment shader, given its type and source code
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader); //Links, compiles, then deletes vertex and fragment shaders. Returns the created program
}
