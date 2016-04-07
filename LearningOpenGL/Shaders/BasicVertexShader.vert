#version 330 core //Declare version, mention we're using core functionality

layout(location = 0) in vec3 position; //Declare all input vertex attributes
//The "layout location" refers to where/how we will be telling this program to load its vertex attributes

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	
}