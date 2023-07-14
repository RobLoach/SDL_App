# SDL_App

Application wrapper for [SDL](https://libsdl.org/) to accomodate desktop, mobile, and web.

## Introduction

When building SDL applications for desktop, mobile, and web, there are subtle differences to accomodate the application workflow. [*SDL_App.h*](include/SDL_App.h) aims to clean up those differences, and make the code easier to read for a unified application entry. This pattern is inspired by [sokol_app](https://github.com/floooh/sokol#sokol_apph).

## Example

``` c
#include <SDL2/SDL.h>

#define SDL_APP_IMPLEMENTATION
#include "SDL_App.h"

typedef struct AppData {
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppData;

SDL_bool Init(void* userData) {
	SDL_Init(SDL_INIT_EVERYTHING);

    AppData* appData = (AppData*)userData;
	appData->window = SDL_CreateWindow("SDL_App: Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, SDL_WINDOW_SHOWN);
	appData>renderer = SDL_CreateRenderer(appData->window, -1, SDL_RENDERER_PRESENTVSYNC);

    return SDL_TRUE;
}

SDL_bool Update(void* userData) {
    AppData* appData = (AppData*)userData;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                return SDL_FALSE;
        }
    }

    SDL_SetRenderDrawColor(appData->renderer, 102, 191, 255, 255);
    SDL_RenderClear(appData->renderer);
    SDL_RenderPresent(appData->renderer);

    return SDL_TRUE;
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
        .userData = SDL_malloc(sizeof(AppData)), // Optional
    };
}
```

## API

### `SDL_App Main(int argc, char* argv[])`

Rather than having your own `int main()`, define your own `Main` function.

``` c
SDL_App Main(int argc, char* argv[]) {
    return (SDL_App) {
        .init = Init,               // The init callback that is called when the application initializes
        .update = Update,           // The update callback that is called when the application should render
        .close = Close,             // The close callback which is called when the application is closed
        .userData = NULL            // Custom user data that is passed through all the callbacks (Optional)
    };
}
```

## License

*SDL_App* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
