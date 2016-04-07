// LearningOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW\glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //Callback function for keyboard input
void loadShader(const char* file);
void loadVertexShader(const char* file);
void loadFragmentShader(const char* file);

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

	//*************
	//VERTEX DATA AND VERTEX BUFFER OBJECT
	//*************

	//Define Normalized Device Coordinates (NDC) for the triangle we want to draw (x, y, z)
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	GLuint VBO; //Create a vertex buffer object
	glGenBuffers(1, &VBO); //Generate its unique ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it to the GL_ARRAY_BUFFER target
										//Now, any calls we make to the GL_ARRAY_BUFFER target will be used to modify the current bound buffer, which is VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Copy vertex/NDC data into data buffer's memory
		//The last agrument for glBufferData defines how we want the GPU to handle/manage the given data.
		//There are 3 forms:
		// 1. GL_STATIC_DRAW: The data will not change, or will do so very rarely
		// 2. GL_DYNAMIC_DRAW: The data will change alot
		// 3. GL_STREAM_DRAW: The data will change every time it is dra wn
		//Changing this allows the GPU to know where to allocate this vertex data into it's memory, so more dynamicly changing data gets access to faster memory

	//*************
	//VERTEX SHADER
	//*************

	//Our shader program, stored as a const GLchar*, soon to be replaced with loadVertexShader()
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";

	GLuint vertexShader; //Shader ID
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //Create a unique shader ID
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Attach the shader source code to the shader
	glCompileShader(vertexShader); //And compile it!
	//Now, check for proper compilation
	GLint successfulCompilation;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successfulCompilation);
	if (!successfulCompilation) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed.\n" << infoLog << std::endl;
	}

	//*************
	//FRAGMENT SHADER
	//*************

	//Our shader program, stored as a const GLchar*, soon to be replaced with loadFragmentShader()
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\n\0";
	//Now allocate, link, and compile fragment shader as before:
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successfulCompilation);
	if (!successfulCompilation) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed.\n" << infoLog << std::endl;
	}

	//*************
	//LINKING SHADERS TO SHADER PROGRAM
	//*************

	GLuint shaderProgram;
	shaderProgram = glCreateProgram(); //Get unique ID for our shader program
	//Attach both vertex and fragment shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //Link 'em!
	//Check for errors:
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successfulCompilation);
	if (!successfulCompilation) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader linking failed.\n" << infoLog << std::endl;
	}
	//We don't need those seperate shaders, so delete them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//The vertex shader allows us to specify any data we want to pass into it, but we need to tell it what part of our input data goes to which attribute
	//IE: "This means we have to specify how OpenGL should interpret the vertex data before rendering."
	//Remember, our vertices are stored as floats (32-bits or 4 bytes each)
	//Each attribute is stored in a buffer, and is tightly packed
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//The arguments for glVertexAttribPointer are as follows:
		//1. The layout position of the attribute (layout = 0, 1, etc)
		//2. Specify the size of the attribute
		//3. Specify the type of data
		//4. Specify if we want the data to be normalized
		//5. Specify the stride, ie: how many bytes between the next vertex attribute
		//6. Specify offset for where the attribute begins in the buffer
	glEnableVertexAttribArray(0); //Enable the attribute at layout location 0

	//*************
	//VERTEX ARRAY OBJECT
	//*************

	GLuint VAO;
	glGenVertexArrays(1, &VAO); //Generate VAO just like we do for a VBO
	glBindVertexArray(VAO); //Bind VAO
	
	//Copy vertices array into a buffer for OGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
	//Then, set set our vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Unbind the VAO
	glBindVertexArray(0);

	//*************
	//MAIN LOOP
	//*************
	while (!glfwWindowShouldClose(window)) { //If we haven't yet instructed the window to close yet
		glfwPollEvents(); //Check if any events are triggered, use callback methods

		//Render things!
		glClearColor(1.0f, 0.6f, 0.3f, 1.0f); //Set the colour for clearning the buffer
		glClear(GL_COLOR_BUFFER_BIT); //Then clear the buffer

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window); //Swap the colour buffer, show it as output to the window
	}
	
	glfwTerminate(); //Clean up resources
	return 0;
}

void loadShader(const char* file) { //NOT DONE
	std::ifstream shader;
	shader.open(file);
	std::string s;
	std::getline(shader, s);
	std::cout << s << std::endl;
	shader.close();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { //If the key that changed was the escape key, and it was pressed
		glfwSetWindowShouldClose(window, GL_TRUE); //Tell the window it was pressed in to close
	}
}