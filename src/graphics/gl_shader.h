#ifndef TEST_GL_SHADER_H
#define TEST_GL_SHADER_H
#include "containers/dynstring.h"
#include "rdefs.h"

typedef struct shadergl {
	uint gl_program;
	uint gl_fragment;
	uint gl_vertex;
} shadergl_t;

// assuming out is allocated
void compileShaderGL(const string_t const *fragment_shader_source,
					 const string_t const *vertex_shader_source, shadergl_t *const out);

#endif // TEST_GL_SHADER_H
