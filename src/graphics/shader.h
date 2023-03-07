//
// Created by csg on 3/6/2023.
//

#ifndef COLF_SHADER_H
#define COLF_SHADER_H
typedef struct {
	unsigned int program;
	unsigned int fragment;
	unsigned int vertex;
} Shader_t;
void initShader(const char *frag_source, const char *vertex_source, Shader_t *out);
#endif // COLF_SHADER_H
