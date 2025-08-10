#pragma once

typedef void (*button_evt)(void*);

struct GoL_Button {
	SDL_FRect pos{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect inner{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect caption_box{ 0.f, 0.f, 0.f, 0.f };
	SDL_Texture* caption = nullptr;
	float caption_scale;
	void* event_param = nullptr;
	button_evt event = nullptr;
	bool pushed = false;
};

void GoL_InitButton(GoL_Button& button, SDL_Texture* caption, button_evt event, void* evt_param, SDL_FRect dimensions, unsigned short border_width = 4, unsigned short padding = 10, bool rescale_caption = true) {
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

void DestroyButton(GoL_Button* button) {
	SDL_DestroyTexture(button->caption);
	SDL_free(button);
}