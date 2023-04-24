#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex_coord;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
out vec2 TexCoord;
void main() {
	vec3 vertex_position_worldspace = vec3(0, 0, 0)
                                      + vec3(view[0][0], view[1][0], view[2][0]) * pos.x
                                      + vec3(view[0][1], view[1][1], view[2][1]) * pos.y;
	gl_Position = projection * view * model * vec4(vertex_position_worldspace, 1.0);
	TexCoord = tex_coord;
}