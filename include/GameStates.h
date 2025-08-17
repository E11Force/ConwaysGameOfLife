#pragma once

//////////////////////////////
//	COMMON STATE DEFINITIONS
//////////////////////////////

enum AppState { MENU, EDITOR, RULES, PREGAME, GAME, EXIT } global_app_state;

static Button* buttons;
static unsigned short button_count;

static Label* labels;
static unsigned short label_count;

void HandleButtons(SDL_Renderer* renderer) {
	static SDL_FPoint mouse_pos;
	static SDL_MouseButtonFlags mouse_buttons;
	mouse_buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < button_count; i++) {
		CheckButtonEvent(buttons[i], mouse_pos, mouse_buttons);
		RenderButton(renderer, buttons[i], mouse_pos);
	}
}

void HandleLabels(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < label_count; i++) {
		RenderLabel(renderer, labels[i]);
	}
}

void ClearScreen(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void BasicStateHandler(SDL_Renderer* renderer) {
	ClearScreen(renderer);
	HandleButtons(renderer);
	HandleLabels(renderer);
}

void UnloadUIResources() {
	for (int i = 0; i < label_count; i++) {
		SDL_DestroyTexture(labels[i].content);
	}
	SDL_free(labels);
	for (int i = 0; i < button_count; i++) {
		SDL_DestroyTexture(buttons[i].content);
	}
	SDL_free(buttons);
	button_count = label_count = 0;
}

//////////////////////////////
//		MENU STATE 
//////////////////////////////

// BUTTON EVENTS

void StartButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::PREGAME;
	UnloadUIResources();
}

void RulesButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::RULES;
	UnloadUIResources();
}

void ExitButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::EXIT;
	UnloadUIResources();
}

// MANAGING RESOURCES

void LoadMenuResources(SDL_Renderer* renderer) {
	buttons = (Button*)SDL_calloc((size_t)3, sizeof(Button));
	button_count = 3;
	labels = (Label*)SDL_calloc((size_t)2, sizeof(Label));
	label_count = 2;

	int renderer_width, renderer_height;
	SDL_GetRenderOutputSize(renderer, &renderer_width, &renderer_height);

	float headers_spacing = 20.f;
	SDL_FPoint headers_position_buffer = {renderer_width / 2.f, 200.f};
	InitLabelText(labels[0], renderer, "Conway's game of life", 100.f, headers_position_buffer, HORIZONTAL_CENTERING);
	
	headers_position_buffer.y += labels[0].content->h + headers_spacing;
	InitLabelText(labels[1], renderer, "by BuBaReK", 20.f, headers_position_buffer, HORIZONTAL_CENTERING);


	float buttons_spacing = 30.f;	
	SDL_FRect buttons_dstrect_buffer = {headers_position_buffer.x - 100.f, 
										headers_position_buffer.y + labels[1].content->h + 100.f,
										200.f, 
										100.f};
	InitButtonText(buttons[0], renderer, buttons_dstrect_buffer, "Start", 30, &StartButtonEvt, &global_app_state, 4, 10);

	buttons_dstrect_buffer.y += buttons_dstrect_buffer.h + buttons_spacing;
	InitButtonText(buttons[1], renderer, buttons_dstrect_buffer, "Rules", 30, &RulesButtonEvt, &global_app_state, 4, 10);

	buttons_dstrect_buffer.y += buttons_dstrect_buffer.h + buttons_spacing;
	InitButtonText(buttons[2], renderer, buttons_dstrect_buffer, "Exit", 30, &ExitButtonEvt, &global_app_state, 4, 10);
}

//////////////////////////////
//	MAIN GAME MANAGEMENT
//////////////////////////////

void InitGame(SDL_Renderer* renderer) {
	global_app_state = AppState::MENU;
	LoadMenuResources(renderer);
	InitAutomata(renderer);
}

SDL_AppResult GameHandler(SDL_Renderer* renderer) {
	switch (global_app_state) {
	case AppState::MENU:
	case AppState::RULES:
		BasicStateHandler(renderer);
		break;
	case AppState::EXIT:
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}