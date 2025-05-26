#pragma once

#include <string.h>
#include <algorithm>

#define DOTO_FONT "fonts\\static\\Doto-Regular.ttf"

struct GoL_Label {
	SDL_FRect pos{ 0.f, 0.f, 0.f, 0.f };
	float scale = 0.f;
	SDL_Texture* label = nullptr;
};

void InitLabelText(GoL_Label* label, SDL_Renderer* renderer, const char* caption, const char* font_file, float text_size, SDL_FPoint position) {
	TTF_Font* font = TTF_OpenFont(font_file, text_size);

	SDL_Surface* caption_surface = TTF_RenderText_Blended(font, caption, strlen(caption), {255, 255, 255, SDL_ALPHA_OPAQUE});
	SDL_Texture* caption_texture = SDL_CreateTextureFromSurface(renderer, caption_surface);
	SDL_DestroySurface(caption_surface);
	TTF_CloseFont(font);

	label->label = caption_texture;
	label->scale = 1.f;
	label->pos.x = position.x;
	label->pos.y = position.y;
	label->pos.h = caption_texture->h;
	label->pos.w = caption_texture->w;
}

void InitLabelImage(GoL_Label* label, SDL_Renderer* renderer, const char* image_file, SDL_FRect dimensions) {
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

void DestroyLabel(GoL_Label* label) {
	SDL_DestroyTexture(label->label);
	SDL_free(label);
}