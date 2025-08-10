#pragma once

//////////////////////////////
//		COMMON UI DEFINITIONS
//////////////////////////////

#define FONT_FILE "fonts\\static\\Doto-Regular.ttf"

SDL_Texture* GetRenderedText(SDL_Renderer* renderer, const char* caption, float text_size) {
	TTF_Font* font = TTF_OpenFont(FONT_FILE, text_size);

	SDL_Surface* text_surface = TTF_RenderText_Blended(font, caption, strlen(caption), { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_DestroySurface(text_surface);
	TTF_CloseFont(font);

	return text_texture;
}

//////////////////////////////
//		LABEL DEFINITIONS
//////////////////////////////

struct Label {
	SDL_FPoint pos{ 0.f, 0.f };
	float scale = 0.f;
	SDL_Texture* label = nullptr;
};

void InitLabelText(Label* label, SDL_Renderer* renderer, const char* caption, float text_size, SDL_FPoint position, bool center_position = false) {
	label->label = GetRenderedText(renderer, caption, text_size);
	label->scale = 1.f;
	label->pos.x = (center_position ? position.x - label->label->w / 2 : position.x);
	label->pos.y = (center_position ? position.y - label->label->h / 2 : position.y);
}

void InitLabelImage(Label* label, SDL_Renderer* renderer, const char* image_file, SDL_FRect dimensions) {
	SDL_Surface* image_surface = SDL_LoadBMP(image_file);
	SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_DestroySurface(image_surface);

	label->label = image_texture;
	label->pos.x = dimensions.x;
	label->pos.y = dimensions.y;
	label->pos.h = fminf(dimensions.h, image_texture->h);
	label->pos.w = fminf(dimensions.w, image_texture->w);
	label->scale = fminf(dimensions.h / label->pos.h, dimensions.w / label->pos.w);
}

void DestroyLabel(Label* label) {
	SDL_DestroyTexture(label->label);
	SDL_free(label);
}

inline void RenderLabel(SDL_Renderer* renderer, Label& label) {
	SDL_RenderTextureTiled(renderer, label.label, nullptr, label.scale, &(label.pos));
}

//////////////////////////////
//		BUTTON DEFINITIONS
//////////////////////////////

typedef void (*button_evt)(void*);

struct Button {
	SDL_FRect pos{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect inner{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect caption_box{ 0.f, 0.f, 0.f, 0.f };
	SDL_Texture* caption = nullptr;
	float caption_scale;
	void* event_param = nullptr;
	button_evt event = nullptr;
	bool pushed = false;
};

void InitButton(Button& button, SDL_Texture* caption, button_evt event, void* evt_param, SDL_FRect dimensions, unsigned short border_width = 4, unsigned short padding = 10, bool rescale_caption = true) {
	button.pos = { dimensions.x, dimensions.y, dimensions.w, dimensions.h };
	button.inner = { button.pos.x + border_width,
					 button.pos.y + border_width,
					 button.pos.w - border_width * 2,
					 button.pos.h - border_width * 2 };
	button.caption = caption;
	SDL_FRect padding_box = { button.inner.x + padding,
							  button.inner.y + padding,
							  button.inner.w - padding * 2,
							  button.inner.h - padding * 2 };
	if (rescale_caption) {
		button.caption_scale = fminf(padding_box.w / button.caption->w, padding_box.h / button.caption->h);
	}
	else {
		button.caption_scale = 1.f;
	}
	button.caption_box.w = button.caption->w * button.caption_scale;
	button.caption_box.h = button.caption->h * button.caption_scale;
	button.caption_box.x = padding_box.x + (padding_box.w - button.caption_box.w) / 2;
	button.caption_box.y = padding_box.y + (padding_box.h - button.caption_box.h) / 2;

	button.event = event;
	button.event_param = evt_param;
}

void DestroyButton(Button* button) {
	SDL_DestroyTexture(button->caption);
	SDL_free(button);
}

SDL_FRect GetButtonHoveredRect(const SDL_FRect& rect) {
	static float scale = 1.3f;
	SDL_FRect hovered = { rect.x - rect.w * (scale - 1.f) / 2,
						  rect.y - rect.h * (scale - 1.f) / 2,
						  rect.w * scale,
						  rect.h * scale };
	return hovered;
}

void RenderButton(SDL_Renderer* renderer, const Button& button) {
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