#pragma once

#include "GoL_Global.h"
#include "GoL_Button.h"

void CheckButtonEvent(GoL_Button& button) {
	static SDL_FPoint mouse_pos;
	SDL_MouseButtonFlags mouse_buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	
	if (SDL_PointInRectFloat(&mouse_pos, &button.pos)) {
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

/*--- Main Menu Buttons ---*/
void StartButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::PREGAME;
}

void RulesButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::RULES;
}

void ExitButtonEvt(void* app_state) {
	*((AppState*)app_state) = AppState::EXIT;
}
/*---------*/

/*--- Rules buttons ---*/
void RulesBackButtonEvt(void* app_state) {

}
/*---------*/
