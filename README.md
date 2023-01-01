# SDL_app

Application wrapper for [SDL](https://libsdl.org/).

## Introduction

When building an SDL application for desktop, mobile, and web, there are subtle differences to accomodate the application workflow. SDL_app aims to clean up those differences, and make the code easier to read for a unified application entry. This pattern is inspired by [sokol_app](https://github.com/floooh/sokol#sokol_apph).

## Example

``` c
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
	appData->renderer = SDL_CreateRenderer(appData->window, -1, SDL_RENDERER_PRESENTVSYNC);
}

void Update(SDL_App* app) {
    AppData* appData = (AppData*)app->userData;
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                SDL_AppClose(app);
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
```

## API

Rather than having your own `int main()`, define your own `App Main(int argc char* argv[])` function.

``` c
App Main(int argc, char* argv[]) {
    return (App) {
        .init = Init,               // The init callback that is called when the application initializes
        .update = Update,           // The update callback that is called when the application should render
        .close = Close,             // The close callback which is called when the application is closed
        .shouldClose = SDL_FALSE,   // Indicate whether or not the application should be closed
        .exitStatus = 0,            // When the application closes, this is the exit status that is returned
        .userData = NULL            // Custom user data that is passed through all the callbacks
    };
}
```

## License

*SDL_app* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.