#include <SDL2/SDL.h>

#define SDL_APP_IMPLEMENTATION
#include "../include/SDL_App.h"

typedef struct AppData {
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppData;

SDL_bool Init(void* userData) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return SDL_FALSE;
    }

    AppData* appData = (AppData*)userData;
	appData->window = SDL_CreateWindow("SDL_App: Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, SDL_WINDOW_SHOWN);
    if (appData->window == NULL) {
        return SDL_FALSE;
    }

	appData->renderer = SDL_CreateRenderer(appData->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (appData->renderer == NULL) {
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

SDL_bool Update(void* userData) {
    AppData* appData = (AppData*)userData;
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                return SDL_FALSE; // Stop the update loop.
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return SDL_FALSE; // Stop the update loop.
                }
                break;
        }
    }

    SDL_SetRenderDrawColor(appData->renderer, 102, 191, 255, 255);
    SDL_RenderClear(appData->renderer);
    SDL_RenderPresent(appData->renderer);

    return SDL_TRUE; // Continue the update loop.
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;
	SDL_DestroyRenderer(appData->renderer);
	SDL_DestroyWindow(appData->window);
	SDL_Quit();
}

SDL_App Main(int argc, char* argv[]) {
    return (SDL_App) {
        .init = Init,
        .update = Update,
        .close = Close,
        .userData = SDL_malloc(sizeof(AppData)),
    };
}
