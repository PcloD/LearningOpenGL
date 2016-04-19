#version 330 core //Declare version, mention we're using core functionality

in vec4 vertexColor; //The input variable coming from the vertex shader. (Same name and type!)

out vec4 color; //Vector4 for colour structure

void main() {
	color = vertexColor;
}