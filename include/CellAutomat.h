#pragma once

#define CELL_SIZE 4 // cell is square with w & h in px

#define PREVIEW_SCALING 2.f

typedef char** cell_field;
typedef char* cell_row;
typedef char cell;

//////////////////////////////
//		CELL AUTOMAT
//////////////////////////////

struct field_config {
	int width, height;
};

struct update_range {
	int start_row;
	int finish_row;
};

void AllocFields();

void DeleteFields();

static int UpdateFieldPart(void* part);

void UpdateField();

void InitUpdateThreads();

void InitAutomata(SDL_Renderer* renderer);

void InitFieldFromBMPImage(const char* image);

//////////////////////////////
//		UI COMPONENTS
//////////////////////////////

struct view_config {
	float scale = 1.f;
	float cell_size = CELL_SIZE;
	SDL_FPoint diff;
	SDL_FRect showed_cells;
};

void UpdateViewRect(SDL_Renderer* renderer);

void RenderField(SDL_Renderer* renderer);

void MoveView(SDL_Renderer* renderer, float diff_x, float diff_y);

void ChangeRenderScale(SDL_Renderer* renderer, float diff_scale);