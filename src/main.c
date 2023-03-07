#include <cglm/cglm.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <glad/glad.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graphics/model.h"
#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "level.h"
#include "stretchy_buffer.h"
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
			for(int i = 0; i < 6; ++i) {
				map.data[MAP_SIZE * y + x].tex_index[i] = rand() % 3;
			}
			if (y == 0 || y == MAP_SIZE - 1 || x == 0 || x == MAP_SIZE - 1 || (y > 4 && y < 8 && x > 4 && x < 8)) {
				map.data[MAP_SIZE * y + x].solid = true;
				map.data[MAP_SIZE * y + x].color.r = rand();
				map.data[MAP_SIZE * y + x].color.g = rand();
				map.data[MAP_SIZE * y + x].color.b = rand();

			}
		}
	}

	StretchyBuffer_t world_vertices = stretchyBufferInit(sizeof(float));
	StretchyBuffer_t world_texture_coordinates = stretchyBufferInit(sizeof(float));

	// build geometry
	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x) {
			// TODO: culling
			// 4 sides
			// 3 * 6
			bool face_visible[6];
			memset(face_visible, 0, sizeof(bool) * 6);
			if (map.data[MAP_SIZE * y + x].solid) {
				// north face
				if (y - 1 >= 0) {
					if (!map.data[MAP_SIZE * (y - 1) + x].solid) {
						face_visible[0] = true;
					}
				}
				// south face
				if (y + 1 < MAP_SIZE) {
					if (!map.data[MAP_SIZE * (y + 1) + x].solid) {
						face_visible[1] = true;
					}
				}

				// west face
				if (x - 1 >= 0) {
					if (!map.data[MAP_SIZE * (y) + x - 1].solid) {
						face_visible[2] = true;
					}
				}
				// east face
				if (x + 1 < MAP_SIZE) {
					if (!map.data[MAP_SIZE * (y) + x + 1].solid) {
						face_visible[3] = true;
					}
				}
			}

			if (!map.data[MAP_SIZE * (y) + x].solid) {
				face_visible[4] = true;
				face_visible[5] = true;
			}

			for (int face = 0; face < 6; ++face) { // face
				if (!face_visible[face]) {
					continue;
				}
				for (int index = 0; index < 6; ++index) {     // inner face
					for (int coord = 0; coord < 3; ++coord) { // coord
						float k = unit_cube_vertices[(face * 18) + (index * 3) + coord];
						float tex = face_texture_coordinates[(face * 18) + (index * 3) + coord];
						switch (coord) {
						case 0:
							k += x;
							break;
						case 1:
							if (!map.data[MAP_SIZE * y + x].solid) {
								k -= 1;
							}
							if(face == 4) {
								k += 2;
							}
							break;
						case 2:
							k += y;
							tex += map.data[MAP_SIZE * y + x].tex_index[face];
							break;
						}
						stretchyBufferPush(&(k), &world_vertices);
						stretchyBufferPush(&(tex), &world_texture_coordinates);
					}
				}
			}
		}
	}

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
	glBufferData(GL_ARRAY_BUFFER, world_vertices.length * sizeof(float), world_vertices.buffer, GL_STATIC_DRAW);

	unsigned int world_texture_coords_buffer;
	glGenBuffers(1, &world_texture_coords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, world_texture_coords_buffer);
	glBufferData(GL_ARRAY_BUFFER, world_texture_coordinates.length * sizeof(float), world_texture_coordinates.buffer, GL_STATIC_DRAW);

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

	mat4 model;
	glm_mat4_identity(model);
	glm_rotate(model, 45, (vec3){0, 1, 0});

	vec3 camera_position = {0, -5.0f, 0.0f};

	SDL_Event ev;
	bool run = true;
	float i = 0;

	unsigned long frame_start = 0;
	unsigned long frame_end = SDL_GetTicks64();
	float delta = 1.0f / 60.0f;
	unsigned long accumulated_frame_time = 0;
	double speed = 50;

	float angle = 0;

	bool input[8] = {
		false, false, false, false, false, false, false, false};

	billboardSpriteInit();

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
				}
			} break;
			}
		}

		while (accumulated_frame_time > 16) {
			//			printf("INPUT\n");
			if (input[0]) {
				camera_position[2] += cosf(-angle) * speed * delta;
				camera_position[0] += sinf(-angle) * speed * delta;
			}

			if (input[1]) {
				camera_position[2] -= cosf(-angle) * speed * delta;
				camera_position[0] -= sinf(-angle) * speed * delta;
			}

			if (input[2]) {
				camera_position[2] -= cosf(-angle + M_PI_2) * speed * delta;
				camera_position[0] -= sinf(-angle + M_PI_2) * speed * delta;
			}

			if (input[3]) {
				camera_position[2] += cosf(-angle + M_PI_2) * speed * delta;
				camera_position[0] += sinf(-angle + M_PI_2) * speed * delta;
			}
			if (input[4]) {
				angle += 1 * delta;
			}
			if (input[5]) {
				angle -= 1 * delta;
			}

			if (input[6]) {
				camera_position[1] -= speed * delta;
			}
			if (input[7]) {
				camera_position[1] += speed * delta;
			}

			accumulated_frame_time -= 16;
		}
		glClearColor(.5, .5, .5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm_mat4_identity(render_data.camera);
		glm_rotate(render_data.camera, angle, (vec3){0, 1, 0});
		glm_translate(render_data.camera, camera_position);

		drawSpriteBillboard(ss, &render_data, 100, 5, 100);
		drawSpriteBillboard(ss, &render_data, 110, 5, 100);
		drawSpriteBillboard(ss, &render_data, 120, 5, 100);
		drawSpriteBillboard(ss, &render_data, 130, 5, 100);
		drawSpriteBillboard(ss, &render_data, 140, 5, 100);

		mat4 model;
		glm_mat4_identity(model);
		glm_scale(model, (vec3){10, 10, 10});
		glm_translate(model, (vec3){0, 0, 0});

		glBindTexture(GL_TEXTURE_2D, sam.index);

		glBindVertexArray(world_vao);
		glUseProgram(flat_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "view"), 1, GL_FALSE, render_data.camera);
		glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "projection"), 1, GL_FALSE, render_data.projection);
		glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "model"), 1, GL_FALSE, model);

		glDrawArrays(GL_TRIANGLES, 0, world_vertices.length / 3);

		//
		//		// build geometry
		//		float scale = 10;
		//		float offset = -100;
		//		for (int y = 0; y < MAP_SIZE; ++y) {
		//			for (int x = 0; x < MAP_SIZE; ++x) {
		//				glm_mat4_identity(model);
		//				RGBColor_t c = map.data[MAP_SIZE * y + x].color;
		//				glUniform3f(glGetUniformLocation(flat_shader.program, "c"), normalize(0, 255, c.r), normalize(0, 255, c.g), normalize(0, 255, c.b));
		//				if (map.data[MAP_SIZE * y + x].solid) {
		//					glm_translate(model, (vec3){x * scale + offset, -(scale / 2.0f), y * scale + offset});
		//					glm_scale_uni(model, 10);
		//				} else {
		//					glm_translate(model, (vec3){x * scale + offset, -(scale * 1.5f), y * scale + offset});
		//					glm_scale_uni(model, 10);
		//				}
		//
		//				glUniformMatrix4fv(glGetUniformLocation(flat_shader.program, "model"), 1, GL_FALSE, model);
		//				//				glDrawArrays(GL_TRIANGLES, 0, 108 / 3);
		//				glDrawArrays(GL_TRIANGLES, 0, 36);
		//			}
		//		}

		SDL_GL_SwapWindow(render_data.window);

		frame_end = SDL_GetTicks64();
		accumulated_frame_time += frame_end - frame_start;
	}

	return 0;
}