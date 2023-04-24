#version 330 core
out vec4 FragColor;
in vec2 tex_coord;
uniform sampler2D inTexture;
void main() {
	FragColor = texture(inTexture, tex_coord);
}