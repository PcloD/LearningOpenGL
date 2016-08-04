// LearningOpenGL.cpp : Defines the entry point for the console application.
//

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

//SOIL (Simple OpenGL Image Library)
#include <SOIL.h>

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //Callback function for keyboard input

int main(){
	//Setup & initializations:
	//*************
	//GLFW initialization
	//*************

	const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
	glfwInit(); //Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Tell GLFW we're using OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW we're going to be using the core profile for OpenGL
	//(This will allow GLFW to throw errors if we accidentally use legacy functions. 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Don't let the user resize the window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr); //Create window object
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Make the context of the window the context on the current thread
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); //Set the viewport for rendering. (Left, bottom, width, height)
	glfwSetKeyCallback(window, key_callback); //Set the key callback function for the main window

	//*************
	//GLEW initialization
	//*************

	glewExperimental = GL_TRUE; //Allow us to use core profile for OpenGL
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	Shader mainShader("Shaders/BasicVertexShader_Passing_Example.vert", "Shaders/BasicFragmentShader_Passing_Example.frag");

	//*************
	//Image & Texture work
	//*************

	int width, height;
	unsigned char* image = SOIL_load_image("noise_cloud.png", &width, &height, 0, SOIL_LOAD_RGB);


	//*************
	//VERTEX DATA,
	//VERTEX BUFFER OBJECT, (VBO)
	//VERTEX ATTRIBUTE OBJECT, (VAO)
	//*************
	GLfloat vertices[] = {
		 // Positions         // Colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top 
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO); //Generate the objects, get their IDs
	glGenBuffers(1, &VBO);
	
	//Bind the vertex array object, then bind and set the vertex buffer and attribute pointers
	glBindVertexArray(VAO);
		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//Bind attribute pointers
			//Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
			//Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	//Unbind VAO
	glBindVertexArray(0);
	
	//*************
	//MAIN LOOP
	//*************
	while (!glfwWindowShouldClose(window)) { //If we haven't yet instructed the window to close yet
		glfwPollEvents(); //Check if any events are triggered, use callback methods

		//Render things!
		glClearColor(1.0f, 0.6f, 0.3f, 1.0f); //Set the colour for clearning the buffer
		glClear(GL_COLOR_BUFFER_BIT); //Then clear the buffer

		//glUseProgram(shaderProgram);
		mainShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window); //Swap the colour buffer, show it as output to the window
	}
	
	glfwTerminate(); //Clean up resources
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { //If the key that changed was the escape key, and it was pressed
		glfwSetWindowShouldClose(window, GL_TRUE); //Tell the window it was pressed in to close
	}
}