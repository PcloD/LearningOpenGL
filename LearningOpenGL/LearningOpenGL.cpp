// LearningOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW\glfw3.h>

//Callback function for keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { //If the key that changed was the escape key, and it was pressed
		glfwSetWindowShouldClose(window, GL_TRUE); //Tell the window it was pressed in to close
	}
}

int main(){
	//Setup & initializations:
	const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

	glfwInit(); //Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Tell GLFW we're using OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW we're going to be using the core profile for OpenGL
	//(This will allow GLFW to throw errors if we accidentally use legacy functions. 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Don't let the user resize the window

	//GLFW initialization
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr); //Create window object
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Make the context of the window the context on the current thread
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); //Set the viewport for rendering. (Left, bottom, width, height)
	glfwSetKeyCallback(window, key_callback); //Set the key callback function for the main window

	//GLEW initialization
	glewExperimental = GL_TRUE; //Allow us to use core profile for OpenGL
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//Main loop:
	while (!glfwWindowShouldClose(window)) { //If we haven't yet instructed the window to close yet
		glfwPollEvents(); //Check if any events are triggered, use callback methods

		//Render things!
		glClearColor(1.0f, 0.6f, 0.3f, 1.0f); //Set the colour for clearning the buffer
		glClear(GL_COLOR_BUFFER_BIT); //Then clear the buffer

		glfwSwapBuffers(window); //Swap the colour buffer, show it as output to the window
	}
	
	glfwTerminate(); //Clean up resources
	return 0;
}