#include <GoL.h>

static cell_field field = nullptr;
static cell_field buffer_field = nullptr;
static field_config size;

void AllocFields() {
	field = (cell_field)SDL_calloc(size.height, sizeof(cell_row));
	buffer_field = (cell_field)SDL_calloc(size.height, sizeof(cell_row));
	for (int i = 0; i < size.height; i++) {
		field[i] = (cell_row)SDL_calloc(size.width, sizeof(cell));
		buffer_field[i] = (cell_row)SDL_calloc(size.width, sizeof(cell));
		for (int j = 0; j < size.width; j++) {
			field[i][j] = 0;
		}
	}
}

void DeleteFields() {
	for (int i = 0; i < size.height; i++) {
		SDL_free(field[i]);
		SDL_free(buffer_field[i]);
	}
	SDL_free(field);
	SDL_free(buffer_field);
}

static int thread_count;
static update_range* ranges;
static thrd_t* threads;

void InitUpdateThreads() {
	thread_count = SDL_GetNumLogicalCPUCores();
	threads = (thrd_t*)SDL_calloc(thread_count, sizeof(thrd_t));
	ranges = (update_range*)SDL_calloc(thread_count, sizeof(update_range));

	int range_diff = size.height / thread_count;
	for (int i = 0; i < thread_count; i++) {
		ranges[i].start_row = i * (range_diff);
		ranges[i].finish_row = (i + 1) * (range_diff);
	}
	ranges[thread_count - 1].finish_row += range_diff % thread_count;
}

void InitAutomata(SDL_Renderer* renderer) {
	int render_width, render_height;
	SDL_GetRenderOutputSize(renderer, &render_width, &render_height);
	size.width = render_width / CELL_SIZE + (render_width % CELL_SIZE != 0);
	size.height = render_height / CELL_SIZE + (render_height % CELL_SIZE != 0);
	AllocFields();

	InitUpdateThreads();
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

static view_config view;

void UpdateViewRect(SDL_Renderer* renderer) {
	view.showed_cells.x = std::min((int)(view.diff.x / view.cell_size), size.width);
	view.showed_cells.y = std::min((int)(view.diff.y / view.cell_size), size.height);
	int renderer_width, renderer_height;
	SDL_GetRenderOutputSize(renderer, &renderer_width, &renderer_height);
	view.showed_cells.w = std::min((int)(renderer_width / view.cell_size) + 1, size.width - (int)view.showed_cells.x);
	view.showed_cells.h = std::min((int)(renderer_height / view.cell_size) + 1, size.height - (int)view.showed_cells.y);
}

void RenderField(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	static SDL_FRect cell_draw_buffer;
	cell_draw_buffer = { view.showed_cells.w * view.cell_size - view.diff.x, 
						 view.showed_cells.h * view.cell_size - view.diff.y, 
						 view.cell_size, 
						 view.cell_size};

	for (int i = view.showed_cells.y; i - view.showed_cells.y < view.showed_cells.h; i++) {
		cell_draw_buffer.y = i * cell_draw_buffer.h;
		for (int j = view.showed_cells.x; j - view.showed_cells.x < view.showed_cells.w; j++) {
			cell_draw_buffer.x = j * cell_draw_buffer.w;
			if (field[i][j]) SDL_RenderFillRect(renderer, &cell_draw_buffer);
		}
	}
}

void MoveView(SDL_Renderer* renderer, float diff_x, float diff_y) {
	int renderer_width, renderer_height;
	SDL_GetRenderOutputSize(renderer, &renderer_width, &renderer_height);
	if(view.diff.x + diff_x + renderer_width < renderer_width * view.scale)
		view.diff.x = fminf(view.diff.x + diff_x, renderer_width * (view.scale - 1));
	if(view.diff.y + diff_y + renderer_height < renderer_height * view.scale)
		view.diff.y = fminf(view.diff.y + diff_y, renderer_height * (view.scale - 1));
	UpdateViewRect(renderer);
}

void ChangeRenderScale(SDL_Renderer* renderer, float diff_scale) {
	int renderer_width, renderer_height;
	SDL_GetRenderOutputSize(renderer, &renderer_width, &renderer_height);
	
	float new_field_width = renderer_width * (view.scale + diff_scale);
	float new_field_height = renderer_height * (view.scale + diff_scale);
	float old_field_width = renderer_width * view.scale;
	float old_field_height = renderer_height * view.scale;
	
	SDL_FPoint new_diff {fmaxf(view.diff.x + (new_field_width - old_field_width) / 2, 0.f), 
						 fmaxf(view.diff.y + (new_field_height - old_field_height) / 2, 0.f)};
	
	if(new_diff.x + renderer_width > new_field_width) 
		new_diff.x = fmaxf(new_diff.x - (new_diff.x + renderer_width - new_field_width), 0.f); 
	if(new_diff.y + renderer_height > new_field_height) 
		new_diff.y = fmaxf(new_diff.y - (new_diff.y + renderer_height - new_field_height), 0.f); 
	view.diff = new_diff;
	view.scale = fmaxf(view.scale + diff_scale, 1.f);
	view.cell_size = CELL_SIZE * view.scale;
	UpdateViewRect(renderer);
}