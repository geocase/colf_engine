#include <cglm/cglm.h>

#include "renderer.h"
#ifdef __APPLE__
#include <SDL2/SDL.h>
#endif

#ifndef __APPLE__
#include <SDL.h>
#endif
#ifdef __APPLE__
#include "OpenGL/gl3.h"
#endif

#ifndef __APPLE__
#include <glad/glad.h>
#endif

void initRenderer(RenderData_t *data) {
	glm_mat4_identity(data->projection);
	glm_mat4_identity(data->ortho);
	glm_mat4_identity(data->camera);
	data->window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 10, 10,
									SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	data->context = SDL_GL_CreateContext(data->window);
	SDL_GL_MakeCurrent(data->window, data->context);
#ifndef __APPLE__
	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		exit(-1);
	}
#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// SDL_GL_SetSwapInterval(1);

	return;
}

void setWindowSize(float w, float h, RenderSettings_t *settings, RenderData_t *data) {
	settings->window_w = w;
	settings->window_h = h;
	glm_perspective(M_PI_2, settings->window_w / settings->window_h, 0.1f, 10, data->projection);
	glm_ortho(0.0f, settings->window_w, settings->window_h, 0.0f, -1.0f, 1.0f, data->ortho);
	glViewport(0, 0, settings->window_w, settings->window_h);
	SDL_SetWindowSize(data->window, w, h);
}

void centerWindow(RenderData_t *data) {
	SDL_SetWindowPosition(data->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}