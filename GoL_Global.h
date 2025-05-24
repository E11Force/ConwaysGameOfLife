#pragma once

#define CELL_SIZE 4
#define FPS 24

enum AppState {MENU, EDITOR, RULES, PREGAME, GAME, EXIT};

typedef char** cell_field;
typedef char* cell_row;
typedef char cell;

struct field_config {
	int render_width, render_height;
	int width, height;
};