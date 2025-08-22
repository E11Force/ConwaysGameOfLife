#include <GoL.h>

SDL_Texture* GetRenderedText(SDL_Renderer* renderer, const char* caption, float text_size) {
	TTF_Font* font = TTF_OpenFont(FONT_FILE, text_size);

	SDL_Surface* text_surface = TTF_RenderText_Blended(font, caption, strlen(caption), { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_DestroySurface(text_surface);
	TTF_CloseFont(font);

	return text_texture;
}

SDL_Texture* LoadBMPImage(SDL_Renderer* renderer, const char* image_file) {
	SDL_Surface* image_surface = SDL_LoadBMP(image_file);
	SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_DestroySurface(image_surface);
	return image_texture;
}

void InitLabelText(Label& label, SDL_Renderer* renderer, const char* caption, float text_size, SDL_FPoint position, Centering center_position = { false, false }) {
	label.content = GetRenderedText(renderer, caption, text_size);
	label.dstrect.x = (center_position.x ? position.x - label.content->w / 2.f : position.x);
	label.dstrect.y = (center_position.y ? position.y - label.content->h / 2.f : position.y);
	label.dstrect.w = (float)label.content->w;
	label.dstrect.h = (float)label.content->h;
}

void InitLabelImage(Label& label, SDL_Renderer* renderer, const char* image_file, SDL_FRect dstrect) {
	label.content = LoadBMPImage(renderer, image_file);
	label.dstrect.x = dstrect.x;
	label.dstrect.y = dstrect.y;
	label.dstrect.w = (dstrect.w < 0 ? label.content->w : dstrect.w);
	label.dstrect.h = (dstrect.h < 0 ? label.content->h : dstrect.h);
}

void DestroyLabel(Label* label) {
	SDL_DestroyTexture(label->content);
	SDL_free(label);
}

static void InitBorderAndRect(Button& button, SDL_FRect dstrect, unsigned char border_width) {
	button.dstrect = dstrect;
	button.border_rect = { button.dstrect.x - border_width,
							button.dstrect.y - border_width,
							button.dstrect.w + border_width * 2,
							button.dstrect.h + border_width * 2 };
}

static void SetConformingContentRect(Button& button, unsigned char padding) {
	SDL_FRect conform_rect = { button.dstrect.x + padding,
							   button.dstrect.y + padding,
							   button.dstrect.w - padding * 2,
							   button.dstrect.h - padding * 2 };
	float width_scale = conform_rect.w / button.content->w;
	float height_scale = conform_rect.h / button.content->h;
	if (button.content->h * width_scale <= conform_rect.h)
		button.content_rect = { conform_rect.x,
								conform_rect.y + (conform_rect.h - button.content->h * width_scale) / 2,
								button.content->w * width_scale,
								button.content->h * width_scale };
	else
		button.content_rect = { conform_rect.x + (conform_rect.w - button.content->w * height_scale) / 2,
								conform_rect.y,
								button.content->w * height_scale,
								button.content->h * height_scale };
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
	button.content = LoadBMPImage(renderer, image_file);
	SetConformingContentRect(button, padding);
	button.event = event;
	button.event_param = evt_param;
}

void DestroyButton(Button* button) {
	SDL_DestroyTexture(button->content);
	SDL_free(button);
}

void CheckButtonEvent(Button& button, SDL_FPoint& mouse_pos, SDL_MouseButtonFlags& mouse_buttons) {
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

void InitSlider(Slider& slider, SDL_Renderer* renderer, SDL_FRect dstrect, SDL_FPoint show_value_pos, float show_value_size, float* change_value, unsigned char border_width) {
	slider.show_value = (Label*)SDL_alloc(sizeof(Label));
	InitLabelText(slider.show_value, renderer);
	slider.change_value = change_value;
	slider.dstrect = dstrect;
	slider.border_rect = { slider.dstrect.x - border_width,
							slider.dstrect.y - border_width,
							slider.dstrect.w + border_width * 2,
							slider.dstrect.h + border_width * 2 };
}

void DestroySlider(Slider* slider) {
	SDL_free(slider->show_value);
	SDL_free(slider);
}

void CheckSlider(Slider& slider, SDL_FPoint& mouse_pos, SDL_MouseButtonFlags& mouse_buttons) {
	
}

void RenderSlider(SDL_Renderer* renderer, Slider& slider) {
	SDL_SetRenderDrawColor(255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &slider.border_rect);
	SDL_SetRenderDrawColor(0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &slider.dstrect);
	
	float slider_position = slider.dstrect.x + slider.dstrect.w * (*slider.change_value / (slider.max_val + slider.min_val));
	SDL_RenderLine(renderer, {slider_position, slider.dstrect.y}, {slider_position, slider.dstrect.y + slider.dstrect.h}, slider.dstrect.x - slider.border_rect.x);
}