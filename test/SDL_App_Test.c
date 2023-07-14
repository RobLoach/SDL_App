#include <SDL2/SDL.h>

#define SDL_APP_IMPLEMENTATION
#include "../include/SDL_App.h"

typedef struct AppData {
    SDL_bool initCalled;
    SDL_bool updateCalled;
    int updateCallCount;
} AppData;

SDL_bool Init(void* userData) {
    AppData* appData = (AppData*)userData;
	appData->initCalled = SDL_TRUE;
    appData->updateCallCount = 0;

    SDL_Log("Init() called");

    return SDL_TRUE;
}

SDL_bool Update(void* userData) {
    AppData* appData = (AppData*)userData;

	appData->updateCalled = SDL_TRUE;
    if (++appData->updateCallCount > 10) {
        return SDL_FALSE;
    }

    SDL_Log("Update() call #%d", appData->updateCallCount);

    return SDL_TRUE;
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;
    SDL_Log("Close() called");
    SDL_assert(appData->initCalled);
    SDL_assert(appData->updateCalled);
    SDL_assert(appData->updateCallCount > 5);
}

SDL_App Main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    return (SDL_App) {
        .init = Init,
        .update = Update,
        .close = Close,
        .userData = SDL_malloc(sizeof(AppData)),
    };
}
