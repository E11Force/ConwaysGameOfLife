#pragma once

//////////////////////////////
//	COMMON STATE DEFINITIONS
//////////////////////////////

enum AppState { MENU, EDITOR, RULES, PREGAME, GAME, EXIT };

void HandleButtons(SDL_Renderer* renderer);

void HandleLabels(SDL_Renderer* renderer);

void ClearScreen(SDL_Renderer* renderer);

void BasicStateHandler(SDL_Renderer* renderer);

void UnloadUIResources();

//////////////////////////////
//		MENU STATE 
//////////////////////////////

// BUTTON EVENTS

void StartButtonEvt(void* app_state);

void RulesButtonEvt(void* app_state);

void ExitButtonEvt(void* app_state);

// MANAGING RESOURCES

void LoadMenuResources(SDL_Renderer* renderer);

//////////////////////////////
//		RULES STATE 
//////////////////////////////

// BUTTON EVENTS

// MANAGING RESOURCES

void LoadRulesResources(SDL_Renderer* renderer);

//////////////////////////////
//	MAIN GAME MANAGEMENT
//////////////////////////////

void InitGame(SDL_Renderer* renderer);

SDL_AppResult GameHandler(SDL_Renderer* renderer);