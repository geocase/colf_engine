#ifndef COLF_RENDERER_H
#define COLF_RENDERER_H
#include <SDL.h>
typedef struct {
	float window_w;
	float window_h;
	float render_w;
	float render_h;
} RenderSettings_t;

typedef struct {
	SDL_Window *window;
	SDL_GLContext *context;
	mat4 projection;
	mat4 ortho;
	mat4 camera;
} RenderData_t;

void initRenderer(RenderData_t *data);
void setWindowSize(float w, float h, RenderSettings_t *settings, RenderData_t *data);
void centerWindow(RenderData_t *data);

#endif // COLF_RENDERER_H
