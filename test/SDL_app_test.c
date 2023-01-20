#include <SDL2/SDL.h>

#define SDL_APP_IMPLEMENTATION
#include "../SDL_app.h"

typedef struct AppData {
    SDL_bool initCalled;
    SDL_bool updateCalled;
} AppData;

void Init(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
	appData->initCalled = SDL_TRUE;
    SDL_Log("Init() called");
}

void Update(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
	appData->updateCalled = SDL_TRUE;
    SDL_AppClose(app);
    SDL_Log("Update() called");
}

void Close(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
    SDL_Log("Close() called");
    SDL_assert(appData->initCalled);
    SDL_assert(appData->updateCalled);
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
