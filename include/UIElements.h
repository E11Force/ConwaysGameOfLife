#pragma once

//////////////////////////////
//		COMMON UI DEFINITIONS
//////////////////////////////

#define FONT_FILE "fonts\\static\\Doto-Regular.ttf"

SDL_Texture* GetRenderedText(SDL_Renderer* renderer, const char* caption, float text_size);

SDL_Texture* LoadBMPImage(SDL_Renderer* renderer, const char* image_file);

struct Centering {
	bool x;
	bool y;
};

#define HORIZONTAL_CENTERING {true, false}
#define VERTICAL_CENTERING {false, true}
#define ABSOLUTE_CENTERING {true, true}
#define NO_CENTERING {false, false}

//////////////////////////////
//		LABEL DEFINITIONS
//////////////////////////////

struct Label {
	SDL_FRect dstrect;
	SDL_Texture* content;
};

void InitLabelText(Label& label, SDL_Renderer* renderer, const char* caption, float text_size, SDL_FPoint position, Centering center_position);

void InitLabelImage(Label& label, SDL_Renderer* renderer, const char* image_file, SDL_FRect dstrect);

void DestroyLabel(Label* label);

inline void RenderLabel(SDL_Renderer* renderer, Label& label) { SDL_RenderTexture(renderer, label.content, nullptr, &label.dstrect); }

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

static void InitBorderAndRect(Button& button, SDL_FRect dstrect, unsigned char border_width);

static void SetConformingContentRect(Button& button, unsigned char padding);

void InitButtonText(Button& button, SDL_Renderer* renderer, SDL_FRect dstrect, const char* caption, float text_size, button_evt event, void* evt_param, unsigned char border_width, unsigned char padding);

void InitButtonImage(Button& button, SDL_Renderer* renderer, SDL_FRect dstrect, const char* image_file, button_evt event, void* evt_param, unsigned char border_width, unsigned char padding);

void DestroyButton(Button* button);

void CheckButtonEvent(Button& button, SDL_FPoint& mouse_pos, SDL_MouseButtonFlags& mouse_buttons);

SDL_FRect GetButtonHoveredRect(const SDL_FRect& rect);

void RenderButton(SDL_Renderer* renderer, const Button& button, SDL_FPoint& mouse_pos);

//////////////////////////////
//		SLIDER DEFINITIONS
//////////////////////////////

struct Slider {
	SDL_FRect SDL_FRect dstrect{ 0.f, 0.f, 0.f, 0.f };
	SDL_FRect border_rect{ 0.f, 0.f, 0.f, 0.f };
	float max_val = 0.f;
	float min_val = 0.f;
	float* change_val = nullptr;
	Label* show_value = nullptr;
	bool engaged = false;
}

void InitSlider(Slider& slider, SDL_Renderer* renderer, SDL_FRect dstrect, SDL_FPoint show_value_pos, float show_value_size, float* change_value, unsigned char border_width);

void DestroySlider(Slider* slider);

void CheckSlider(Slider& slider, SDL_FPoint& mouse_pos, SDL_MouseButtonFlags& mouse_buttons);

void RenderSlider(SDL_Renderer* renderer, Slider& slider);