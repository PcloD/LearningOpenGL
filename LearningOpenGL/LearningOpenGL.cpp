// LearningOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW\glfw3.h>

int main(){
	//Setup & initializations:
	int width = 800, height = 600;

	glfwInit(); //Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Tell GLFW we're using OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW we're going to be using the core profile for OpenGL
	//(This will allow GLFW to throw errors if we accidentally use legacy functions. 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Don't let the user resize the window

	//GLFW initialization
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr); //Create window object
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Make the context of the window the context on the current thread
	glViewport(0, 0, width, height); //Set the viewport for rendering. (Left, bottom, width, height)

	//GLEW initialization
	glewExperimental = GL_TRUE; //Allow us to use core profile for OpenGL
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}



	return 0;
}