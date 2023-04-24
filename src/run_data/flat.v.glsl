#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 text;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 c;
out vec2 tex_coord;
void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	tex_coord = vec2(((text.x + text.z) / 3), text.y); // temp
}