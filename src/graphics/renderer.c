#include <cglm/cglm.h>

#include <SDL.h>
#include <glad/glad.h>
#include "renderer.h"

void initRenderer(RenderData_t *data) {
	glm_mat4_identity(data->projection);
	glm_mat4_identity(data->camera);
	data->window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 10, 10, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	data->context = SDL_GL_CreateContext(data->window);
	SDL_GL_MakeCurrent(data->window, data->context);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		exit(-1);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return;
}

void setWindowSize(float w, float h, RenderSettings_t *settings, RenderData_t *data) {
	settings->window_w = w;
	settings->window_h = h;
	glm_perspective(1.22173, settings->window_w / settings->window_h, 1.0f, 400.0f, data->projection);
	glViewport(0, 0, settings->window_w, settings->window_h);
	SDL_SetWindowSize(data->window, w, h);
}

void centerWindow(RenderData_t *data) {
	SDL_SetWindowPosition(data->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}