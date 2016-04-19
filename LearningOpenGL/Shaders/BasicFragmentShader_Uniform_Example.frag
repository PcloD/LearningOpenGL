#version 330 core //Declare version, mention we're using core functionality

out vec4 color; //Vector4 for colour structure

uniform vec4 ourColor;

void main() {
	color = ourColor;
}