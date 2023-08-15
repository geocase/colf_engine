#ifdef __APPLE__
#include "OpenGL/gl3.h"
#endif

#ifndef __APPLE__
#include <glad/glad.h>
#endif 
#include <stdlib.h>
#include <stdio.h>
#include "gl_shader.h"

void compileShaderGL(const string_t const *fragment_shader_source, const string_t const *vertex_shader_source,
					 shadergl_t *const out) {
	out->gl_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(out->gl_vertex, 1, &vertex_shader_source->str, NULL);
	glCompileShader(out->gl_vertex);
	int success;
	glGetShaderiv(out->gl_vertex, GL_COMPILE_STATUS, &success);
	if(!success) {
		int length;
		glGetShaderiv(out->gl_vertex, GL_INFO_LOG_LENGTH, &length);
		char* info_log = malloc(sizeof(char) * length);
		glGetShaderInfoLog(out->gl_vertex, length, NULL, info_log);
		printf("Failed to compile gl vertex shader:\n\t%s\n", info_log);
		printf("Input shader:\n%s\n", vertex_shader_source->str);
		free(info_log);
	}

	out->gl_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(out->gl_fragment, 1, &fragment_shader_source->str, NULL);
	glCompileShader(out->gl_fragment);
	glGetShaderiv(out->gl_fragment, GL_COMPILE_STATUS, &success);
	if(!success) {
		int length;
		glGetShaderiv(out->gl_fragment, GL_INFO_LOG_LENGTH, &length);
		char* info_log = malloc(sizeof(char) * length);
		glGetShaderInfoLog(out->gl_fragment, length, NULL, info_log);
		// send these to a logging function
		printf("Failed to compile gl fragment shader:\n\t%s\n", info_log);
		printf("Input shader:\n%s\n", fragment_shader_source->str);
		free(info_log);
	}

	out->gl_program = glCreateProgram();
	glAttachShader(out->gl_program, out->gl_vertex);
	glAttachShader(out->gl_program, out->gl_fragment);
	glLinkProgram(out->gl_program);
	glGetShaderiv(out->gl_program, GL_LINK_STATUS, &success);
	if(!success) {
		int length;
		glGetShaderiv(out->gl_fragment, GL_INFO_LOG_LENGTH, &length);
		char* info_log = malloc(sizeof(char) * length);
		glGetProgramInfoLog(out->gl_program, length, NULL, info_log);
		// send these to a logging function
		printf("Failed to compile shader program:\n\t%s\n", info_log);
		free(info_log);
	}
	return;
}
