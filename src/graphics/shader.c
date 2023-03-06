#include <glad/glad.h>
#include <stdio.h>

#include "shader.h"

void initShader(const char *frag_source, const char *vertex_source, Shader_t *out) {
	out->vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(out->vertex, 1, &vertex_source, NULL);
	glCompileShader(out->vertex);

	int success;
	char infoLog[512];
	glGetShaderiv(out->vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(out->vertex, 512, NULL, infoLog);
		printf("VERT\n%s\n", infoLog);
		return;
	}

	out->fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(out->fragment, 1, &frag_source, NULL);
	glCompileShader(out->fragment);

	glGetShaderiv(out->fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(out->fragment, 512, NULL, infoLog);
		printf("FRAG\n%s\n", infoLog);
		return;
	}

	out->program = glCreateProgram();
	glAttachShader(out->program, out->vertex);
	glAttachShader(out->program, out->fragment);
	glLinkProgram(out->program);

	glGetShaderiv(out->program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(out->program, 512, NULL, infoLog);
		printf("%s\n", infoLog);
		return;
	}
}