#pragma once

#include "ShaderManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ShaderManager {
	std::string loadFile(const char* file) {
		std::ifstream ifs;
		ifs.open(file); //Attempt to open the file

		if (ifs.good()) { //If it opened successfully
			std::ostringstream oss; //Create a string stream for outputing into
			std::string temp; //Create a temp string for storing each line
			while (ifs.good()) { //While we can read from the file, ie: not EOF
				getline(ifs, temp); //Get the next line
				oss << temp << '\n'; //Concatinate a new line to the end
			}
			ifs.close(); //Now close the file
			return oss.str(); //And return the corresponding string
		}
		//Otherwise, output and error and return an empty string
		std::cerr << "File: " << file << " invalid or not found." << std::endl;
		return "";
	}


	GLuint compileShader(GLenum shaderType, std::string shaderCode) {
		if (shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER) {
			const GLchar* shaderSource = shaderCode.c_str();
			GLuint shaderID;
			shaderID = glCreateShader(shaderType);
			glShaderSource(shaderID, 1, &shaderSource, NULL); //Attach shader source code to the shader
			glCompileShader(shaderID); //Compile that code

									   //Check for sucessful compilation
			GLint successfulCompilation;
			GLchar infoLog[512];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successfulCompilation);
			if (!successfulCompilation) { //If we didn't sucessfully compile, spit out some error code
				glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
				if (shaderType == GL_VERTEX_SHADER) {
					std::cerr << "Vertex shader compilation failed.\n" << infoLog << std::endl;
				} else {
					std::cerr << "Fragment shader compilation failed.\n" << infoLog << std::endl;
				}
				
				
				return NULL;
			}
			else { //Otherwise,
				return shaderID; //Return the ID for the sucessfully compiled shader
			}
		}
		else {
			std::cerr << "Given shader type not compilable yet." << std::endl;
		}
	}

	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
		GLuint shaderProgramID;
		shaderProgramID = glCreateProgram(); //Get unique ID for our shader program
											 //Attach both vertex and fragment shaders
		glAttachShader(shaderProgramID, vertexShader);
		glAttachShader(shaderProgramID, fragmentShader);
		glLinkProgram(shaderProgramID); //Link 'em!
										//Check for errors:
		GLint successfulCompilation;
		GLchar infoLog[512];
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &successfulCompilation);
		if (!successfulCompilation) { //If we didn't sucessfully compile, spit out some error code 
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			std::cerr << "Shader linking failed.\n" << infoLog << std::endl;
			return NULL;
		}
		else { //Otherwise,
			   //We don't need those seperate shaders, so delete them.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return shaderProgramID; //Return the compiled shader
		}
	}
}