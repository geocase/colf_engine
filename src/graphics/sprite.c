#include <cglm/cglm.h>
#ifdef __APPLE__
#include "OpenGL/gl3.h"
#endif

#ifndef __APPLE__
#include <glad/glad.h>
#endif 

#include "renderer.h"
#include "gl_shader.h"
#include "sprite.h"
#include "texture.h"
#include "disk.h"
#include "containers/dynstring.h"

shadergl_t billboard_shader;
unsigned int billboard_vao;

shadergl_t hud_shader;
unsigned int hud_vao;

void billboardSpriteInit() {
	// resource ids?
	string_t bb_fs = readTextFile("run_data/billboard.f.glsl");
	string_t bb_vs = readTextFile("run_data/billboard.v.glsl");
	compileShaderGL(&bb_fs, &bb_vs, &billboard_shader);

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
	// convert this to a model
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
	glUseProgram(billboard_shader.gl_program);
	glUniformMatrix4fv(glGetUniformLocation(billboard_shader.gl_program, "model"), 1, GL_FALSE, model);
	glUniformMatrix4fv(glGetUniformLocation(billboard_shader.gl_program, "view"), 1, GL_FALSE, render_data->camera);
	glUniformMatrix4fv(glGetUniformLocation(billboard_shader.gl_program, "projection"), 1, GL_FALSE, render_data->projection);
	glBindTexture(GL_TEXTURE_2D, texture.index);

	glBindVertexArray(billboard_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void hudSpriteInit() {
	string_t hud_fs = readTextFile("run_data/hud.f.glsl");
	string_t hud_vs = readTextFile("run_data/hud.v.glsl");
	compileShaderGL(&hud_fs, &hud_vs, &hud_shader);
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

	glUseProgram(hud_shader.gl_program);
	glUniformMatrix4fv(glGetUniformLocation(hud_shader.gl_program, "model"), 1, GL_FALSE, model);
	glUniformMatrix4fv(glGetUniformLocation(hud_shader.gl_program, "orthographic"), 1, GL_FALSE, render_data->ortho);
	glBindTexture(GL_TEXTURE_2D, texture.index);
	// this should always be drawn last
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(hud_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}
