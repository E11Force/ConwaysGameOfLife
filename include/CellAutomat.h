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
	int width, height;
};

static field_config size;

void AllocFields() {
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

void DeleteFields() {
	for (int i = 0; i < size.height; i++) {
		free(field[i]);
		free(buffer_field[i]);
	}
	free(field);
	free(buffer_field);
}

struct update_range {
	int start_row;
	int finish_row;
};

static int UpdateFieldPart(void* part) {
	update_range* update_part = (update_range*)part;
	for (int i = update_part->start_row; i < update_part->finish_row; i++) {
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
	return 0;
}

void UpdateField() {
	static const int thread_count = 6;
	thrd_t threads[thread_count];
	update_range ranges[thread_count];

	int range_diff = size.height / thread_count;
	for (int i = 0; i < thread_count; i++) {
		ranges[i].start_row = i * (range_diff);
		ranges[i].finish_row = (i + 1) * (range_diff);
	}
	ranges[thread_count - 1].finish_row += range_diff % 6;

	for (int i = 0; i < thread_count; i++) {
		thrd_create(&threads[i], &UpdateFieldPart, &ranges[i]);
	}

	for (int i = 0; i < thread_count; i++) {
		int res_stub;
		thrd_join(threads[i], &res_stub);
	}

	cell_field change_buffer = field;
	field = buffer_field;
	buffer_field = change_buffer;
}

bool InitAutomata(SDL_Renderer* renderer) {
	int render_width, render_height;
	SDL_GetRenderOutputSize(renderer, &render_width, &render_height);

	if (render_height % CELL_SIZE != 0 || render_width % CELL_SIZE != 0) return false;

	size.width = render_width / CELL_SIZE;
	size.height = render_height / CELL_SIZE;
	AllocFields();
	return true;
}

void InitFieldFromBMPImage(const char* image) {
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

void RenderField(SDL_Renderer* renderer) {
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