//
// Created by csg on 3/6/2023.
//

#ifndef PLATFORMER_SHADER_H
#define PLATFORMER_SHADER_H
typedef struct {
	unsigned int program;
	unsigned int fragment;
	unsigned int vertex;
} Shader_t;
void initShader(const char *frag_source, const char *vertex_source, Shader_t *out);
#endif // PLATFORMER_SHADER_H
