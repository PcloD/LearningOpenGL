#version 330 core //Declare version, mention we're using core functionality

in vec3 vertexColor; //The input variable coming from the vertex shader. (Same name and type!)
out vec4 color; //Vector4 for colour structure

void main() {
	color = vec4(vertexColor, 1.0f);
}