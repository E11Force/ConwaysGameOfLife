#pragma once

void GoL_ClearScreen(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

SDL_FRect GetButtonHoveredRect(const SDL_FRect& rect) {
	static float scale = 1.3f;
	SDL_FRect hovered = { rect.x - rect.w * (scale - 1.f) / 2,
						  rect.y - rect.h * (scale - 1.f) / 2,
						  rect.w * scale,
						  rect.h * scale };
	return hovered;
}

void GoL_RenderButton(SDL_Renderer* renderer, const GoL_Button& button) {
	static SDL_FPoint mouse_pos;
	SDL_MouseButtonFlags mouse_buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	
	if (!SDL_PointInRectFloat(&mouse_pos, &(button.pos)) || button.pushed) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &(button.pos));
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &(button.inner));
	}
	else {
		SDL_FRect border = GetButtonHoveredRect(button.pos);
		SDL_FRect inner = GetButtonHoveredRect(button.inner);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &border);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &inner);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderTextureTiled(renderer, button.caption, nullptr, button.caption_scale, &button.caption_box);
}

void GoL_RenderField(const cell_field& field, const field_config& size, SDL_Renderer* renderer) {
	GoL_ClearScreen(renderer);
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

void GoL_RenderMenu(SDL_Renderer* renderer, const field_config& size, GoL_Button* buttons, unsigned short button_count, GoL_Label* labels, unsigned short label_count) {
	GoL_ClearScreen(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	for (int i = 0; i < button_count; i++) {
		GoL_RenderButton(renderer, buttons[i]);
	}

	for (int i = 0; i < label_count; i++) {
		SDL_RenderTextureTiled(renderer, labels[i].label, nullptr, labels[i].scale, &(labels[i].pos));
	}
}