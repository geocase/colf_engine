#include <cglm/cglm.h>
#ifdef __APPLE__
#include "OpenGL/gl3.h"
#endif

#ifndef __APPLE__
#include <glad/glad.h>
#endif 

#include "renderer.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"
const char *billboard_vertex_shader = "#version 330 core\n"
									  "layout (location = 0) in vec2 pos;"
									  "layout (location = 1) in vec2 tex_coord;"
									  "uniform mat4 projection;"
									  "uniform mat4 model;"
									  "uniform mat4 view;"
									  "out vec2 TexCoord;"
									  "void main() {"
									  "	vec3 vertex_position_worldspace = vec3(0, 0, 0)"
									  "                                      + vec3(view[0][0], view[1][0], view[2][0]) * pos.x"
									  "                                      + vec3(view[0][1], view[1][1], view[2][1]) * pos.y;"
									  "	gl_Position = projection * view * model * vec4(vertex_position_worldspace, 1.0);"
									  "	TexCoord = tex_coord;"
									  "}";

const char *billboard_fragment_shader = "#version 330 core\n"
										"out vec4 FragColor;"
										"in vec3 color;"
										"in vec2 TexCoord;"
										"uniform sampler2D inTexture;"
										"void main() {"
										"	FragColor = texture(inTexture, TexCoord);"
										"	if(FragColor.a <= 0) { discard; }"
										"}";

const char *hud_vertex_shader = "#version 330 core\n"
								"layout (location = 0) in vec2 pos;"
								"layout (location = 1) in vec2 tex_coord;"
								"uniform mat4 orthographic;"
								"uniform mat4 model;"
								"out vec2 TexCoord;"
								"void main() {"
								"	gl_Position = orthographic * model * vec4(pos, 0, 1.0);"
								"	TexCoord = tex_coord;"
								"}";

const char *hud_fragment_shader = "#version 330 core\n"
								  "out vec4 FragColor;"
								  "in vec2 TexCoord;"
								  "uniform sampler2D inTexture;"
								  "void main() {"
								  "	FragColor = texture(inTexture, TexCoord);"
								  "}";

Shader_t billboard_shader;
unsigned int billboard_vao;

Shader_t hud_shader;
unsigned int hud_vao;

void billboardSpriteInit() {
	initShader(billboard_fragment_shader, billboard_vertex_shader, &billboard_shader);
	float vertices[] = {
		-1.0f,
		1.0f,
		1.0f,
		-1.0f,
		-1.0f,
		-1.0f,

		-1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		-1.0f,
	};

	float tex_coords[] = {
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		0.0f,
	};

	unsigned int vert_buffer;
	glGenBuffers(1, &vert_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);

	glGenVertexArrays(1, &billboard_vao);
	glBindVertexArray(billboard_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);

	return;
}

void drawSpriteBillboard(glTexture_t texture, RenderData_t *render_data, float x, float y, float z) {
	mat4 model;
	float scale = 1.0f; // TODO: this should be stored as a global variable or something
	glm_mat4_identity(model);
	glm_translate(model, (vec3){x, y, z});
	glm_scale(model, (vec3){scale / 2, scale / 2, scale / 2});
	glUseProgram(billboard_shader.program);
	glUniformMatrix4fv(glGetUniformLocation(billboard_shader.program, "model"), 1, GL_FALSE, model);
	glUniformMatrix4fv(glGetUniformLocation(billboard_shader.program, "view"), 1, GL_FALSE, render_data->camera);
	glUniformMatrix4fv(glGetUniformLocation(billboard_shader.program, "projection"), 1, GL_FALSE, render_data->projection);
	glBindTexture(GL_TEXTURE_2D, texture.index);

	glBindVertexArray(billboard_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void hudSpriteInit() {
	initShader(hud_fragment_shader, hud_vertex_shader, &hud_shader);
	float vertices[] = {
		0.0f,
		0.0f,

		1.0f,
		0.0f,

		0.0f,
		1.0f,

		1.0f,
		1.0f,

		0.0f,
		1.0f,

		1.0f,
		0.0f,
		};

	float tex_coords[] = {
		0.0f,
		1.0f,

		1.0f,
		1.0f,

		0.0f,
		0.0f,

		1.0f,
		0.0f,

		0.0f,
		0.0f,

		1.0f,
		1.0f,
	};
	// snip
	unsigned int vert_buffer;
	glGenBuffers(1, &vert_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int tex_coord_buffer;
	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);

	glGenVertexArrays(1, &hud_vao);
	glBindVertexArray(hud_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
}

void drawSpriteHud(glTexture_t texture, RenderData_t *render_data, float x, float y, float w, float h) {
	mat4 model;
	glm_mat4_identity(model);
	glm_translate(model, (vec3){x, y, 0});
//	glm_rotate(model, M_PI_2, (vec3){0, 0, 0});
	glm_scale(model, (vec3){w, h, 1});

	glUseProgram(hud_shader.program);
	glUniformMatrix4fv(glGetUniformLocation(hud_shader.program, "model"), 1, GL_FALSE, model);
	glUniformMatrix4fv(glGetUniformLocation(hud_shader.program, "orthographic"), 1, GL_FALSE, render_data->ortho);
	glBindTexture(GL_TEXTURE_2D, texture.index);
	// this should always be drawn last
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(hud_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}
