#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex_coord;
uniform mat4 orthographic;
uniform mat4 model;
out vec2 TexCoord;
void main() {
	gl_Position = orthographic * model * vec4(pos, 0, 1.0);
	TexCoord = tex_coord;
}