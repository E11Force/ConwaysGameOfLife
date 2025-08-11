#pragma once

//////////////////////////////
//	COMMON STATE DEFINITIONS
//////////////////////////////

enum AppState { MENU, EDITOR, RULES, PREGAME, GAME, EXIT };

static Button* buttons;
static unsigned short button_count;

static Label* labels;
static unsigned short label_count;

void HandleButtons(SDL_Renderer* renderer) {
	for (int i = 0; i < button_count; i++) {
		CheckButtonEvent(buttons[i]);
		RenderButton(renderer, buttons[i]);
	}
}

void HandleLabels(SDL_Renderer* renderer) {
	for (int i = 0; i < label_count; i++) {
		RenderLabel(renderer, labels[i]);
	}
}

void ClearScreen(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

//////////////////////////////
//	MAIN GAME RESOURCE INITIALIZATION
//////////////////////////////

void InitGame() {
	
}

//////////////////////////////
//		MENU STATE 
//////////////////////////////

// BUTTON EVENTS

void StartButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::PREGAME;
}

void RulesButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::RULES;
}

void ExitButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::EXIT;
}

// MANAGING RESOURCES

void LoadMenuResources(SDL_Renderer* renderer, void* app_state, const field_config& size) {
	buttons = (Button*)SDL_calloc((size_t)3, sizeof(Button));
	button_count = 3;
	labels = (Label*)SDL_calloc((size_t)2, sizeof(Label));
	label_count = 2;

	float headers_top = 200.f;
	float headers_spacing = 20.f;
	InitLabelText(&labels[0], renderer, "Corwan's Game of Life", 60, { size.render_width / 2.f, headers_top + 30 }, true);

	InitLabelText(&labels[1], renderer, "by BuBaReK", 20, { size.render_width / 2.f, headers_top + labels[0].label->h + headers_spacing + 10 }, true);
	TTF_Font* header_font = TTF_OpenFont(FONT_FILE, 30);
	float buttons_top = headers_top + labels[0].pos.h + headers_spacing * 2 + labels[1].pos.h + 220.f;
	float buttons_spacing = 130.f;
	SDL_Surface* startbuttoncaption_surface = TTF_RenderText_Blended(header_font, "Start", 5, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* startbuttoncaption_texture = SDL_CreateTextureFromSurface(renderer, startbuttoncaption_surface);
	SDL_DestroySurface(startbuttoncaption_surface);
	SDL_FRect button_dimensions = { (float)size.render_width / 2 - 100,
									buttons_top,
									200,
									100 };
	InitButton(buttons[0], startbuttoncaption_texture, &StartButtonEvt, app_state, button_dimensions);

	SDL_Surface* rulesbuttoncaption_surface = TTF_RenderText_Blended(header_font, "Rules", 5, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* rulesbuttoncaption_texture = SDL_CreateTextureFromSurface(renderer, rulesbuttoncaption_surface);
	SDL_DestroySurface(rulesbuttoncaption_surface);
	button_dimensions = { (float)size.render_width / 2 - 100,
						  buttons_top + buttons_spacing,
						  200,
						  100 };
	InitButton(buttons[1], rulesbuttoncaption_texture, &RulesButtonEvt, app_state, button_dimensions);

	SDL_Surface* exitbuttoncaption_surface = TTF_RenderText_Blended(header_font, "Exit", 4, { 255, 255, 255, SDL_ALPHA_OPAQUE });
	SDL_Texture* exitbuttoncaption_texture = SDL_CreateTextureFromSurface(renderer, exitbuttoncaption_surface);
	SDL_DestroySurface(exitbuttoncaption_surface);
	button_dimensions = { (float)size.render_width / 2 - 100,
						  buttons_top + 2 * buttons_spacing,
						  200,
						  100 };
	InitButton(buttons[2], exitbuttoncaption_texture, &ExitButtonEvt, app_state, button_dimensions);
}

void UnloadMenuResources() {
	for (int i = 0; i < label_count; i++) {
		SDL_DestroyTexture(labels[i].label);
	}
	SDL_free(labels);
	for (int i = 0; i < button_count; i++) {
		SDL_DestroyTexture(buttons[i].caption);
	}
	SDL_free(buttons);
}

void GoL_MenuFlow(SDL_Renderer* renderer, const field_config& size) {
	ClearScreen(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	HandleButtons(renderer);
	HandleLabels(renderer);
}

/*

/*--- Rules Flow ---*/

void GoL_LoadRulesResources(SDL_Renderer* renderer, void* app_state, const field_config& size) {

}

void GoL_UnloadRulesResources() {

}

void GoL_RulesFlow(SDL_Renderer* renderer, const field_config& size) {

}

/*---------*/

/*--- Rules Flow ---*/

void GoL_LoadPregameResources(SDL_Renderer* renderer, void* app_state, const field_config& size) {

}

void GoL_UnloadPregameResources() {

}

void GoL_PregameFlow(SDL_Renderer* renderer, const field_config& size) {

}

/*---------*/

/*--- Rules Flow ---*/

void GoL_LoadEditorResources(SDL_Renderer* renderer, void* app_state, const field_config& size) {

}

void GoL_UnloadEditorResources() {

}

void GoL_EditorFlow(SDL_Renderer* renderer, const field_config& size) {

}

*/