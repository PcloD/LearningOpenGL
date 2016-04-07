// LearningOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW\glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //Callback function for keyboard input
std::string loadFile(const char* file); //Returns string containing contents of given file
GLuint compileShader(GLenum shaderType, std::string shaderCode); //Compiles vertex or fragment shader, given its type and source code
GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader); //Links, compiles, then deletes vertex and fragment shaders. Returns the created program

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

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, loadFile("Shaders/BasicVertexShader.vert"));
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, loadFile("Shaders/BasicFragmentShader.frag"));
	GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

	//*************
	//VERTEX DATA,
	//VERTEX BUFFER OBJECT, (VBO)
	//VERTEX ATTRIBUTE OBJECT, (VAO)
	//ELEMENT BUFFER OBJECT (EBO)
	//*************
	GLfloat vertices[] = { //Rectangle vertices
		 0.5f,  0.5f, 0.0f,  // Top Right
		 0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left 
	};
	GLuint indices[] = {  // Draw order for indexed drawing
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); //Generate the objects, get their IDs
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	//Bind the vertex array object, then bind and set the vertex buffer and attribute pointers
	glBindVertexArray(VAO);
		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//Bind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//Bind attribute pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window); //Swap the colour buffer, show it as output to the window
	}
	
	glfwTerminate(); //Clean up resources
	return 0;
}

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
			std::cerr << "Vertex shader compilation failed.\n" << infoLog << std::endl;
			return NULL;
		} else { //Otherwise,
			return shaderID; //Return the ID for the sucessfully compiled shader
		}
	} else {
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
	} else { //Otherwise,
		//We don't need those seperate shaders, so delete them.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgramID; //Return the compiled shader
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { //If the key that changed was the escape key, and it was pressed
		glfwSetWindowShouldClose(window, GL_TRUE); //Tell the window it was pressed in to close
	}
}