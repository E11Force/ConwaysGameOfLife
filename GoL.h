#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "GoL_Global.h"

static SDL_FRect cell_draw_buffer;
static cell_field buffer_field;

void alloc_field(const field_config& size, cell_field& field) {
	field = (cell_field)calloc(size.height, sizeof(cell_row));
	for (int i = 0; i < size.height; i++) {
		field[i] = (cell_row)calloc(size.width, sizeof(cell));
		for (int j = 0; j < size.width; j++) {
			field[i][j] = 0;
		}
	}
}

void delete_field(const field_config& size, cell_field field) {
	for (int i = 0; i < size.height; i++) {
		free(field[i]);
	}
	free(field);
}

bool GoL_init(cell_field& field_state, field_config& size, SDL_Renderer* renderer) {
	SDL_GetRenderOutputSize(renderer, &size.render_width, &size.render_height);

	if (size.render_height % CELL_SIZE != 0 || size.render_width % CELL_SIZE != 0) return false;
	
	size.width = size.render_width / CELL_SIZE;
	size.height = size.render_height / CELL_SIZE;
	alloc_field(size, field_state);
	alloc_field(size, buffer_field);
	return true;
}

void Init_field_from_bmp(cell_field field, field_config size, const char* image) {
	SDL_Surface* image_buffer = SDL_LoadBMP(image);
	if (!image_buffer) {
		field[0][1] = 1;
		field[1][2] = 1; /* _*_ */
		field[2][0] = 1; /* __* */
		field[2][1] = 1; /* *** */
		field[2][2] = 1;
		return;
	}
	SDL_Surface* resulting_buffer;
	if (image_buffer->format != SDL_PIXELFORMAT_RGBA8888) {
		resulting_buffer = SDL_ConvertSurface(image_buffer, SDL_PIXELFORMAT_RGBA8888);
		SDL_DestroySurface(image_buffer);
	}
	else {
		resulting_buffer = image_buffer;
	}
	for (int i = 0; i < resulting_buffer->h; i++) {
		Uint32* pixels = (Uint32*)(((Uint8*)resulting_buffer->pixels) + (i * resulting_buffer->pitch));
		for (int j = 0; j < resulting_buffer->w; j++) {
			if (pixels[j] == 0x000000FF) {
				field[i][j] = 0;
			}
			else {
				field[i][j] = 1;
			}
		}
	}
}

void GoL_CheckField(cell_field& field, field_config size) {
	static cell_field buffer_field = nullptr;
	if (buffer_field == nullptr) alloc_field(size, buffer_field);
	
	for (int i = 0; i < size.height; i++) {
		for (int j = 0; j < size.width; j++) {
			buffer_field[i][j] = 0;
			for (int k = -1; k <= 1; k++) {
				for (int m = -1; m <= 1; m++) {
					if (m == 0 && k == 0) continue;
					buffer_field[i][j] += (((i + k < 0 || i + k >= size.height) || (j + m < 0 || j + m >= size.width)) ? 0 : field[i + k][j + m]);
				}
			}
			if (buffer_field[i][j] == 3 || (field[i][j] == 1 && buffer_field[i][j] == 2)) {
				buffer_field[i][j] = 1;
			}
			else {
				buffer_field[i][j] = 0;
			}
		}
	}
	
	cell_field change_buffer = field;
	field = buffer_field;
	buffer_field = change_buffer;
}

