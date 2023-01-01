#include <SDL2/SDL.h>

#define SDL_APP_IMPLEMENTATION
#include "SDL_app.h"

typedef struct AppData {
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppData;

void Init(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
	SDL_Init(SDL_INIT_EVERYTHING);
	appData->window = SDL_CreateWindow("SDL_app: Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, SDL_WINDOW_SHOWN);
	appData->renderer = SDL_CreateRenderer(appData->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Update(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                SDL_AppClose(app);
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        SDL_AppClose(app);
                    break;
                }
                break;
        }
    }

    SDL_SetRenderDrawColor(appData->renderer, 102, 191, 255, 255);
    SDL_RenderClear(appData->renderer);
    SDL_RenderPresent(appData->renderer);
}

void Close(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
	SDL_DestroyRenderer(appData->renderer);
	SDL_DestroyWindow(appData->window);
	SDL_Quit();
}

SDL_App Main(int argc, char* argv[]) {
    AppData* appData = SDL_malloc(sizeof(AppData));
    return (SDL_App) {
        .init = Init,
        .update = Update,
        .close = Close,
        .userData = appData,
    };
}
