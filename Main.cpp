#define SDL_MAIN_USE_CALLBACKS 1
#include "GoL.h"

#define FPS 24

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize TTF: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    /* Create the window */
#ifdef _DEBUG
    if (!SDL_CreateWindowAndRenderer("Game of Life by BuBaReK", 1200, 800, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
#elif NDEBUG
    if (!SDL_CreateWindowAndRenderer("Game of Life by BuBaReK", 1200, 800, SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
#endif
    InitGame(renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    const Uint64 before = SDL_GetTicks();

    switch (GameHandler(renderer)) {
    case SDL_AppResult::SDL_APP_SUCCESS:
        return SDL_AppResult::SDL_APP_SUCCESS;
    }
    SDL_RenderPresent(renderer);

    const Uint64 after = SDL_GetTicks();
    static int frame_millis = 1000 / FPS;
    int frame_diff = after - before;
    if (frame_diff < frame_millis) SDL_Delay(frame_millis - frame_diff);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}