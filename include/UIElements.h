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

SDL_Texture* LoadBMPImage(const char* image_file) {
	SDL_Surface* image_surface = SDL_LoadBMP(image_file);
	SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_DestroySurface(image_surface);
	return image_texture;
}

//////////////////////////////
//		LABEL DEFINITIONS
//////////////////////////////

struct Label {
	SDL_FRect dstrect;
	SDL_Texture* content;
};

void InitLabelText(Label& label, SDL_Renderer* renderer, const char* caption, float text_size, SDL_FPoint position, bool center_position = false) {
	label.content = GetRenderedText(renderer, caption, text_size);
	label.dstrect.x = (center_position ? position.x - label.content->w / 2 : position.x);
	label.dstrect.y = (center_position ? position.y - label.content->h / 2 : position.y);
	label.dstrect.w = label.content->w;
	label.dstrect.h = label.content->h;
}

void InitLabelImage(Label& label, SDL_Renderer* renderer, const char* image_file, SDL_FRect dstrect) {
	label.content = LoadBMPImage(image_file);
	label.dstrect.x = dstrect.x;
	label.dstrect.y = dstrect.y;
	label.dstrect.w = (dstrect.w < 0 ? label.content->w : dstrect.w);
	label.dstrect.h = (dstrect.h < 0 ? label.content->h : dstrect.h);
}

void DestroyLabel(Label* label) {
	SDL_DestroyTexture(label->content);
	SDL_free(label);
}

inline void RenderLabel(Label& label, SDL_Renderer* renderer) {
	SDL_RenderTexture(renderer, label.content, nullptr, &label.dstrect);
}

//////////////////////////////
//		BUTTON DEFINITIONS
//////////////////////////////

typedef void (*button_evt)(void*);

struct Button {
	SDL_FRect dstrect{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect border_rect{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect content_rect{ 0.f, 0.f, 0.f, 0.f };
	SDL_Texture* content = nullptr;
	void* event_param = nullptr;
	button_evt event = nullptr;
	bool pushed = false;
};

static void InitBorderAndRect(Button& button, SDL_FRect dstrect, unsigned char border_width) {
	button.dstrect = dstrect;
	button.border_rect = {  button.dstrect.x - border_width,
							button.dstrect.y - border_width,
							button.dstrect.w + border_width * 2,
							button.dstrect.h + border_width * 2};
}

static void SetConformingContentRect(Button& button, unsigned char padding) {
	SDL_FRect conform_rect = { button.dstrect.x + padding,
							   button.dstrect.y + padding,
							   button.dstrect.w - padding * 2,
							   button.dstrect.h - padding * 2 };
	float width_scale = conform_rect.w / button.content.w;
	float height_scale = conform_rect.h / button.content.h;
	if(button.content.h * width_scale <= conform_rect.h)
		button.content_rect = { conform_rect.x,
								conform_rect.y - (conform_rect.h - button.content.h * width_scale) / 2,
								button.content.w * width_scale,
								button.content.h * width_scale };
	else
		button.content_rect = { conform_rect.x - (conform_rect.w - button.content.w * height_scale) / 2,
								conform_rect.y,
								button.content.w * height_scale,
								button.content.h * height_scale};
}

void InitButtonText(Button& button, SDL_Renderer* renderer, SDL_FRect dstrect, const char* caption, float text_size, button_evt event = nullptr, void* evt_param = nullptr, unsigned char border_width = 4, unsigned char padding = 10) {
	InitBorderAndRect(button, dstrect, border_width);
	button.content = GetRenderedText(renderer, caption, text_size);
	SetConformingContentRect(button, padding);
	button.event = event;
	button.event_param = evt_param;
}

void InitButtonImage(Button& button, SDL_Renderer* renderer, SDL_FRect dstrect, const char* image_file, button_evt event = nullptr, void* evt_param = nullptr, unsigned char border_width = 4, unsigned char padding = 10) {
	InitBorderAndRect(button, dstrect, border_width);
	button.content = LoadBMPImage(image_file);
	SetConformingContentRect(button, padding);
	button.event = event;
	button.event_param = evt_param;
}

void DestroyButton(Button* button) {
	SDL_DestroyTexture(button->content);
	SDL_free(button);
}

void CheckButtonEvent(Button& button) {
	static SDL_FPoint mouse_pos;
	SDL_MouseButtonFlags mouse_buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	if (SDL_PointInRectFloat(&mouse_pos, &button.dstrect)) {
		if ((mouse_buttons & SDL_BUTTON_LEFT) && !button.pushed) {
			button.pushed = true;
		}
		else if (!(mouse_buttons & SDL_BUTTON_LEFT) && button.pushed) {
			button.pushed = false;
			(*button.event)(button.event_param);
		}
	}
	else if (!(mouse_buttons & SDL_BUTTON_LEFT)) {
		button.pushed = false;
	}
}

SDL_FRect GetButtonHoveredRect(const SDL_FRect& rect) {
	static float scale = 1.3f;
	SDL_FRect hovered = { rect.x - rect.w * (scale - 1.f) / 2,
						  rect.y - rect.h * (scale - 1.f) / 2,
						  rect.w * scale,
						  rect.h * scale };
	return hovered;
}

void RenderButton(SDL_Renderer* renderer, const Button& button, SDL_FPoint& mouse_pos) {
	static SDL_FRect border_rect;
	static SDL_FRect dstrect;
	
	if (!SDL_PointInRectFloat(&mouse_pos, &(button.dstrect)) || button.pushed) {
		border_rect = button.border_rect;
		dstrect = button.dstrect;
	}
	else {
		border_rect = GetButtonHoveredRect(button.border_rect);
		dstrect = GetButtonHoveredRect(button.dstrect);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &border_rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &dstrect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderTexture(renderer, button.content, nullptr, &button.content_rect);
}