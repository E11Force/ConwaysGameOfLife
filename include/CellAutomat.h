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

void RenderField(SDL_Renderer* renderer);

void RenderPreview(SDL_Renderer* renderer, const SDL_FPoint& pos);