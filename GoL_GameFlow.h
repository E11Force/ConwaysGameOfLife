#pragma once

#include "GoL_Global.h"
#include "GoL_Button.h"
#include "GoL_Renderers.h"
#include "GoL_ButtonEvents.h"

static GoL_Button* buttons;
static unsigned short button_count;

static GoL_Label* labels;
static unsigned short label_count;

void GoL_LoadMenuResources(SDL_Renderer* renderer, void* app_state, const field_config& size) {
	buttons = (GoL_Button*)SDL_calloc((size_t)3, sizeof(GoL_Button));
	button_count = 3;
	labels = (GoL_Label*)SDL_calloc((size_t)2, sizeof(GoL_Label));
	label_count = 2;

	TTF_Font* header_font = TTF_OpenFont("fonts\\static\\Doto-Regular.ttf", 48);

	float headers_top = 200;
	float headers_spacing = 20;
	SDL_Surface* header_surface = TTF_RenderText_Blended(header_font, "Corwans Game of Life", 20, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* header_texture = SDL_CreateTextureFromSurface(renderer, header_surface);
	SDL_DestroySurface(header_surface);
	labels[0].label = header_texture;
	labels[0].scale = 2.5f;
	labels[0].pos = { size.render_width / 2 - header_texture->w * labels[0].scale / 2, 
					  headers_top, 
					  header_texture->w * labels[0].scale, 
					  header_texture->h * labels[0].scale };


	SDL_Surface* subheader_surface = TTF_RenderText_Blended(header_font, "by BuBaReK", 10, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* subheader_texture = SDL_CreateTextureFromSurface(renderer, subheader_surface);
	SDL_DestroySurface(subheader_surface);
	labels[1].label = subheader_texture;
	labels[1].scale = 1.5f;
	labels[1].pos = { size.render_width / 2 - subheader_texture->w * labels[1].scale / 2, 
					  headers_top + header_texture->h + headers_spacing + subheader_texture->h, 
					  subheader_texture->w * labels[1].scale, 
				      subheader_texture->h * labels[1].scale };

	float buttons_top = headers_top + header_texture->h + headers_spacing * 2 + subheader_texture->h + 220.f;
	float buttons_spacing = 130.f;
	SDL_Surface* startbuttoncaption_surface = TTF_RenderText_Blended(header_font, "Start", 5, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* startbuttoncaption_texture = SDL_CreateTextureFromSurface(renderer, startbuttoncaption_surface);
	SDL_DestroySurface(startbuttoncaption_surface);
	SDL_FRect button_dimensions = { (float)size.render_width / 2 - 100,
									buttons_top,
									200,
									100 };
	GoL_InitButton(buttons[0], startbuttoncaption_texture, &StartButtonEvt, app_state, button_dimensions);

	SDL_Surface* rulesbuttoncaption_surface = TTF_RenderText_Blended(header_font, "Rules", 5, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* rulesbuttoncaption_texture = SDL_CreateTextureFromSurface(renderer, rulesbuttoncaption_surface);
	SDL_DestroySurface(rulesbuttoncaption_surface);
	button_dimensions = { (float)size.render_width / 2 - 100,
					      buttons_top + buttons_spacing,
						  200,
						  100 };
	GoL_InitButton(buttons[1], rulesbuttoncaption_texture, &RulesButtonEvt, app_state, button_dimensions);

	SDL_Surface* exitbuttoncaption_surface = TTF_RenderText_Blended(header_font, "Exit", 4, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* exitbuttoncaption_texture = SDL_CreateTextureFromSurface(renderer, exitbuttoncaption_surface);
	SDL_DestroySurface(exitbuttoncaption_surface);
	button_dimensions = { (float)size.render_width / 2 - 100,
						  buttons_top + 2*buttons_spacing,
						  200,
						  100 };
	GoL_InitButton(buttons[2], exitbuttoncaption_texture, &ExitButtonEvt, app_state, button_dimensions);
}

void GoL_UnloadMenuResources() {
	for (int i = 0; i < label_count; i++) {
		SDL_DestroyTexture(labels[i].label);
	}
	SDL_free(labels);
	for (int i = 0; i < button_count; i++) {
		SDL_DestroyTexture(buttons[i].caption);
	}
	SDL_free(buttons);
}

void HandleButtons() {
	for (int i = 0; i < button_count; i++) {
		CheckButtonEvent(buttons[i]);
	}
}

void GoL_MenuFlow(SDL_Renderer* renderer, const field_config& size) {
	HandleButtons();
	GoL_RenderMenu(renderer, size, buttons, button_count, labels, label_count);
}