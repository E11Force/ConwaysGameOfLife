#pragma once

#define CELL_SIZE 4 // cell is square with w & h in px

typedef char** cell_field;
typedef char* cell_row;
typedef char cell;

//////////////////////////////
//		CELL AUTOMAT
//////////////////////////////

static cell_field field = nullptr;
static cell_field buffer_field = nullptr;

struct field_config {
	int render_width, render_height;
	int width, height;
};

void AllocFields(const field_config& size) {
	field = (cell_field)calloc(size.height, sizeof(cell_row));
	buffer_field = (cell_field)calloc(size.height, sizeof(cell_row));
	for (int i = 0; i < size.height; i++) {
		field[i] = (cell_row)calloc(size.width, sizeof(cell));
		buffer_field[i] = (cell_row)calloc(size.width, sizeof(cell));
		for (int j = 0; j < size.width; j++) {
			field[i][j] = 0;
		}
	}
}

void DeleteFields(const field_config& size, cell_field field) {
	for (int i = 0; i < size.height; i++) {
		free(field[i]);
		free(buffer_field[i]);
	}
	free(field);
	free(buffer_field);
}

void UpdateField(cell_field& field, field_config size) {
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

bool InitAutomata(cell_field& field_state, field_config& size, SDL_Renderer* renderer) {
	SDL_GetRenderOutputSize(renderer, &size.render_width, &size.render_height);

	if (size.render_height % CELL_SIZE != 0 || size.render_width % CELL_SIZE != 0) return false;

	size.width = size.render_width / CELL_SIZE;
	size.height = size.render_height / CELL_SIZE;
	AllocFields(size);
	return true;
}

void InitFieldFromBMPImage(cell_field field, field_config size, const char* image) {
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

void RenderField(const field_config& size, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	static SDL_FRect cell_draw_buffer{ 0.f, 0.f, (float)CELL_SIZE, (float)CELL_SIZE };

	for (int i = 0; i < size.height; i++) {
		cell_draw_buffer.y = i * CELL_SIZE;
		for (int j = 0; j < size.width; j++) {
			cell_draw_buffer.x = j * CELL_SIZE;
			if (field[i][j]) SDL_RenderFillRect(renderer, &cell_draw_buffer);
		}
	}
}