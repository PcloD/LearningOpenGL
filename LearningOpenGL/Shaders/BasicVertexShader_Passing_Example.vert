#version 330 core //Declare version, mention we're using core functionality

layout(location = 0) in vec3 position; //Declare all input vertex attributes
//The "layout location" refers to where/how we will be telling this program to load its vertex attributes

out vec4 vertexColor; //Specify a colour we'll output to the fragment shader

void main() {
	gl_Position = vec4(position, 1.0); //Notice how we're using a vec3 inside of a vec4's constructor
	vertexColor = vec4(0.5, 0.0f, 0.0f, 1.0f); //Set the output variable
}