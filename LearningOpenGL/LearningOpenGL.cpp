// LearningOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW\glfw3.h>

int main(){
	glfwInit(); //Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Tell GLFW we're using OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW we're going to be using the Core Profile for OpenGL
	//(This will allow GLFW to throw errors if we accidentally use legacy functions. 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Don't let the user resize the window

	//GLFW initialization
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr); //Create window object
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Make the context of the window the context on the current thread

	//GLEW


	return 0;
}