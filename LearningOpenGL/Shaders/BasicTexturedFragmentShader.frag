#version 330 core
in vec3 ourColor;
in vec2 TexCoord; //Take in the texture co-ordinates

out vec4 color;

uniform sampler2D ourTexture;

void main() {
	color = texture(ourTexture, TexCoord); //The outputed color is the filtered and interpolated (ie. sampled) colour of the given texture
}