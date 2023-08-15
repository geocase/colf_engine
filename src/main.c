#include <cglm/cglm.h>

#define SDL_MAIN_HANDLED
#ifndef __APPLE__
#include <SDL.h>
#endif
#ifdef __APPLE___
#include <SDL2/SDL.h>
#endif
#include <SDL_image.h>
// #include <SDL_mixer.h>
#ifdef __APPLE__
#include "OpenGL/gl3.h"
#endif

#ifndef __APPLE__
#include <glad/glad.h>
#endif 

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "containers/stretchy_buffer.h"
#include "game/entity_functions.h"
#include "game/level.h"
#include "graphics/model.h"
#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "utils.h"

const char *vert = "#version 330 core\n"
				   "layout (location = 0) in vec3 pos;"
				   "layout (location = 1) in vec3 text;"
				   "uniform mat4 projection;"
				   "uniform mat4 model;"
				   "uniform mat4 view;"
				   "uniform vec3 c;"
				   "out vec2 tex_coord;"
				   "void main() {"
				   "	gl_Position = projection * view * model * vec4(pos, 1.0);"
				   "	tex_coord = vec2(((text.x + text.z) / 3), text.y);" // temp
				   "}";

const char *frag = "#version 330 core\n"
				   "out vec4 FragColor;"
				   "in vec2 tex_coord;"
				   "uniform sampler2D inTexture;"
				   "void main() {"
				   "	FragColor = texture(inTexture, tex_coord);"
				   "}";

int main(int argc, char **argv) {
	srand(time(NULL));

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_WEBP | IMG_INIT_PNG);

	// Gen Level
	Map_t map;
	memset(&map, 0, sizeof(Map_t));
	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x) {
			for (int i = 0; i < 6; ++i) {
				map.data[MAP_SIZE * y + x].tex_index[i] = rand() % 3;
			}
			if (y == 0 || y == MAP_SIZE - 1 || x == 0 || x == MAP_SIZE - 1 || (y > 4 && y < 8 && x > 4 && x < 8)) {
				if (!(x == 6 && y < 7)) {
					map.data[MAP_SIZE * y + x].solid = true;
					map.data[MAP_SIZE * y + x].color.r = rand();
					map.data[MAP_SIZE * y + x].color.g = rand();
					map.data[MAP_SIZE * y + x].color.b = rand();
				}
			}
		}
	}

	TexturedModel_t world = generateLevelGeometry(&map);

	// audio junk
	//	Mix_Init(MIX_INIT_OGG);
	//	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
	//		printf("%s\n", SDL_GetError());
	//	}
	//	Mix_Chunk* beep = Mix_LoadWAV("beep.wav");
	//	if(beep == NULL) {
	//		printf("%s\n", SDL_GetError());
	//	}
	//	Mix_Volume(0, 1);
	//	Mix_Volume(1, 1);
	//	Mix_SetPanning(0, 0, 0);
	//	Mix_PlayChannel(0, beep, -1);

	RenderSettings_t render_settings;
	RenderData_t render_data;
	initRenderer(&render_data);

	setWindowSize(1280, 720, &render_settings, &render_data);
	centerWindow(&render_data);

	Shader_t flat_shader;
	initShader(frag, vert, &flat_shader);


	unsigned int world_vert_buffer;
	glGenBuffers(1, &world_vert_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, world_vert_buffer);
	glBufferData(GL_ARRAY_BUFFER, world.tris * 3 * sizeof(float), world.vertices, GL_STATIC_DRAW);

	unsigned int world_texture_coords_buffer;
	glGenBuffers(1, &world_texture_coords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, world_texture_coords_buffer);
	glBufferData(GL_ARRAY_BUFFER, world.tris * 3 * sizeof(float), world.texture_coordinates, GL_STATIC_DRAW);

	unsigned int world_vao;
	glGenVertexArrays(1, &world_vao);
	glBindVertexArray(world_vao);

	glBindBuffer(GL_ARRAY_BUFFER, world_vert_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, world_texture_coords_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);

	// texture
	TextureData_t ss_pixels;
	loadImage("test.png", &ss_pixels);
	glTexture_t ss = pushTextureToGPU(&ss_pixels);

	TextureData_t sam_pixels;
	loadImage("wall_temp.png", &sam_pixels);
	glTexture_t sam = pushTextureToGPU(&sam_pixels);

	TextureData_t bullet_pixels;
	loadImage("bullet.png", &bullet_pixels);
	glTexture_t bullet = pushTextureToGPU(&bullet_pixels);

	TextureData_t sam_real_pixels;
	loadImage("blask.png", &sam_real_pixels);
	glTexture_t sam_real = pushTextureToGPU(&sam_real_pixels);

	TextureData_t gun_real_pixels;
	loadImage("gun.png", &gun_real_pixels);
	glTexture_t gun_real = pushTextureToGPU(&gun_real_pixels);

	mat4 model;
	glm_mat4_identity(model);
	glm_rotate(model, 45, (vec3){0, 1, 0});

	float world_scale = 1.0f;

	vec3 camera_position = {-140, -(world_scale / 2.0f), -80}; // todo: scale here

	SDL_Event ev;
	bool run = true;
	float i = 0;

	unsigned long frame_start = 0;
	unsigned long frame_end = SDL_GetTicks64();
	float delta = 1.0f / 60.0f;
	unsigned long accumulated_frame_time = 0;
	double speed = 1;

	float angle = 0;

	bool input[10] = {
		false, false, false, false, false, false, false, false, false};

	billboardSpriteInit();
	hudSpriteInit();

	Entity_t bullet_entity = {.position = {0, 0}};
	bullet_entity.radius = .5;
	bool bullet_shot = false;

	Entity_t ss_officer = {.position = {10, 6.5}, .angle = 0, .radius = .2};
	Entity_t player = {.position = {1.5, 1.5}, .angle = 0, .radius = .2};

	bool free_cam = false;
	vec2 free_cam_coords;
	float free_cam_angle;

	while (run) {
		//		float volume = max(0, 255 - glm_vec3_distance(camera, (vec3){0, 0, 0}));
		//		Mix_SetPanning(0, volume, volume); // since target is at zero, zero, use camera position only

		frame_start = SDL_GetTicks64();
		i += .001;
		while (SDL_PollEvent(&ev) != 0) {
			switch (ev.type) {
			case SDL_WINDOWEVENT:
				switch (ev.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					setWindowSize(ev.window.data1, ev.window.data2, &render_settings, &render_data);
					break;
				}
				break;
			case SDL_QUIT:
				run = false;
				break;
			case SDL_KEYDOWN:
				if (ev.key.repeat) {
					break;
				}
				switch (ev.key.keysym.sym) {
				case SDLK_w:
					input[0] = true;
					break;
				case SDLK_s:
					input[1] = true;
					break;
				case SDLK_d:
					input[2] = true;
					break;
				case SDLK_a:
					input[3] = true;
					break;
				case SDLK_RIGHT:
					input[4] = true;
					break;
				case SDLK_LEFT:
					input[5] = true;
					break;
				case SDLK_LSHIFT:
					input[6] = true;
					break;
				case SDLK_LCTRL:
					input[7] = true;
					break;
				case SDLK_SPACE:
					input[8] = true;
					// SHOOT BULLET
					{
						bullet_entity.position[0] = player.position[0];
						bullet_entity.position[1] = player.position[1];
						bullet_shot = true;
						bullet_entity.angle = player.angle;
						printf("%f\n", bullet_entity.angle);
						fflush(stdout);
					}
					break;
				case SDLK_j:
					input[9] = true;
					{
						if(!free_cam) {
							free_cam_coords[0] = player.position[0];
							free_cam_coords[1] = player.position[1];
							free_cam_angle = player.angle;
							free_cam = true;
						} else {
							free_cam = false;
						}
					}
					break;
				}

				break;
			case SDL_KEYUP: {
				switch (ev.key.keysym.sym) {
				case SDLK_w:
					input[0] = false;
					break;
				case SDLK_s:
					input[1] = false;
					break;
				case SDLK_d:
					input[2] = false;
					break;
				case SDLK_a:
					input[3] = false;
					break;
				case SDLK_RIGHT:
					input[4] = false;
					break;
				case SDLK_LEFT:
					input[5] = false;
					break;
				case SDLK_LSHIFT:
					input[6] = false;
					break;
				case SDLK_LCTRL:
					input[7] = false;
					break;
				case SDLK_SPACE:
					input[8] = false;
					break;
				case SDLK_j:
					input[9] = false;
					break;
				}
			} break;
			}
		}

		while (accumulated_frame_time > 16) {
			//			entityWalkForward(&ss_officer, &map, .5 * delta);
			entityWalkTowardsPoint(&ss_officer, &map, .5 * delta, player.position);
			if (bullet_shot) {
				entityWalkForward(&bullet_entity, &map, .5 * delta);
			}
			//			printf("INPUT\n");
			if(!free_cam) {
				if (input[0]) {
					entityWalk(&player, &map, speed * delta, player.angle);
				}

				if (input[1]) {
					entityWalk(&player, &map, speed * delta, player.angle + M_PI);
				}

				if (input[2]) {
					entityWalk(&player, &map, speed * delta, player.angle + M_PI_2);
				}

				if (input[3]) {
					entityWalk(&player, &map, speed * delta, player.angle - M_PI_2);
				}
				if (input[4]) {
					player.angle += 1 * delta;
				}
				if (input[5]) {
					player.angle -= 1 * delta;
				}
			} else {
				if (input[0]) {
					free_cam_coords[0] += cosf(free_cam_angle) * 0.4 * delta;
					free_cam_coords[1] += sinf(free_cam_angle) * 0.4 * delta;
				}

				if (input[1]) {
				}

				if (input[2]) {
				}

				if (input[3]) {
				}
				if (input[4]) {
					free_cam_angle += 1 * delta;
				}
				if (input[5]) {
					free_cam_angle -= 1 * delta;
				}
			}

			//			if (input[6]) {
			//				camera_position[1] -= speed * delta;
			//			}
			//			if (input[7]) {
			//				camera_position[1] += speed * delta;
			//			}
			accumulated_frame_time -= 16;
		}

		glClearColor(.5, .5, .5, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT);

		float camera_angle = 0.0f;
		if(free_cam) {
			camera_position[0] = -free_cam_coords[0] * world_scale;
			camera_position[2] = -free_cam_coords[1] * world_scale;
			camera_angle = free_cam_angle;
		} else {
			camera_position[0] = -player.position[0] * world_scale;
			camera_position[2] = -player.position[1] * world_scale;
			camera_angle = player.angle;
		}

		glm_mat4_identity(render_data.camera);
		glm_rotate(render_data.camera, -camera_angle - M_PI_2, (vec3){0, -1, 0});
		glm_translate(render_data.camera, camera_position);

		drawSpriteBillboard(bullet, &render_data, bullet_entity.position[0] * world_scale, world_scale / 2.0f, bullet_entity.position[1] * world_scale); // TODO: scale here as well

		drawSpriteBillboard(ss, &render_data, ss_officer.position[0] * world_scale, world_scale / 2.0f, ss_officer.position[1] * world_scale); // TODO: scale here as well

		mat4 model;
		glm_mat4_identity(model);
		//  these values need to be the same
		glm_scale(model, (vec3){world_scale, world_scale, world_scale}); // TODO: pull scale from a global variable -> stored in sprite.c as well
		glm_translate(model, (vec3){0, 0, 0});

		glBindTexture(GL_TEXTURE_2D, sam.index);

		glBindVertexArray(world_vao);
		glUseProgram(flat_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "view"), 1, GL_FALSE, render_data.camera);
		glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "projection"), 1, GL_FALSE, render_data.projection);
		glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "model"), 1, GL_FALSE, model);

		glDrawArrays(GL_TRIANGLES, 0, world.tris);

		if(!free_cam) {
			drawSpriteHud(gun_real, &render_data, render_settings.window_w / 2 - (render_settings.window_h / 2), 0, render_settings.window_h, render_settings.window_h);
		} else {
			drawSpriteBillboard(sam_real, &render_data, player.position[0] * world_scale, world_scale / 2.0f, player.position[1] * world_scale); // TODO: scale here as well
		}

		SDL_GL_SwapWindow(render_data.window);

		frame_end = SDL_GetTicks64();
		accumulated_frame_time += frame_end - frame_start;
	}

	return 0;
}