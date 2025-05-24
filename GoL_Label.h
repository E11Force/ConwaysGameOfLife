#pragma once

struct GoL_Label {
	SDL_FRect pos{ 0.f, 0.f, 0.f, 0.f };
	float scale = 0.f;
	SDL_Texture* label = nullptr;
};

void InitLabel(GoL_Label* inited, const char* caption, const char* font_file, float text_size, SDL_FPoint position) {

}

void InitLabel(GoL_Label* inited, const char* image_file, SDL_FRect dimensions) {

}

void DestroyLabel(GoL_Label* label) {
	SDL_DestroyTexture(label->label);
	SDL_free(label);
}