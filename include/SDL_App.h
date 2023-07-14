/**********************************************************************************************
*
*   SDL_App v1.0.0 - Application wrapper for SDL.
*
*       https://github.com/RobLoach/SDL_App
*
*   LICENSE: zlib/libpng
*
*   SDL_App is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2023 Rob Loach (@RobLoach)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef SDL_APP_H_
#define SDL_APP_H_

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Application data that is used to manage the window.
 */
typedef struct SDL_App {
    /**
     * The initiatialization callback for the application.
     *
     * @param userData The SDL_App user data that was passed during initialization.
     *
     * @return SDL_TRUE if the application was initialized successfully, SDL_FALSE otherwise.
     */
    SDL_bool (*init)(void* userData);

    /**
     * The update callback to update and draw the application.
     *
     * @param userData The SDL_App user data that was passed during initialization.
     *
     * @return SDL_TRUE to continue running the application, SDL_FALSE to close.
     */
    SDL_bool (*update)(void* userData);

    /**
     * The close callback used to deinitialize all application data.
     *
     * @param userData The SDL_App user data that was passed during initialization.
     */
    void (*close)(void* userData);

    /**
     * Custom user data that is passed through the application callbacks.
     *
     * This is helpful if you have context that needs to be passed through all the callbacks.
     *
     * The memory allocated as part of this will be freed for you.
     */
    void* userData;
} SDL_App;

#if defined(__cplusplus)
}
#endif

#endif  // SDL_APP_H_

#ifdef SDL_APP_IMPLEMENTATION
#ifndef SDL_APP_IMPLEMENTATION_ONCE
#define SDL_APP_IMPLEMENTATION_ONCE

#ifndef SDL_APP_MAIN
/**
 * The name of the entry point in your application.
 *
 * Will default to "Main".
 *
 * @code
 * #define SDL_APP_MAIN MyOwnMain
 * #define SDL_APP_IMPLEMENTATION
 * #include "SDL_App.h"
 *
 * SDL_App MyOwnMain(int argc, char* argv[]) {
 *   // Stuff
 * }
 * @endcode
 **/
#define SDL_APP_MAIN Main
#endif  // SDL_APP_MAIN

// emscripten.h
#if defined(EMSCRIPTEN)
#ifndef SDL_APP_EMSCRIPTEN_H
#define SDL_APP_EMSCRIPTEN_H "emscripten.h"
#endif
#include SDL_APP_EMSCRIPTEN_H
#endif  // EMSCRIPTEN

#ifndef NULL
#include <stddef.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(SDL_APP_NO_ENTRY)
/**
 * The main entry point defining the application behavior.
 *
 * By default, this will be named "Main", but can be changed by defining SDL_APP_MAIN.
 *
 * @param argc The length of the argument vector.
 * @param argv The array of arguments.
 * @example examples/SDL_App_Example.c
 *
 * @return The SDL_App description for your application.
 *
 * @see SDL_App
 * @see SDL_APP_MAIN
 */
extern SDL_App SDL_APP_MAIN(int argc, char* argv[]);
#endif

#if defined(EMSCRIPTEN)
/**
 * The update callback for web.
 */
void SDL_App_Update(void* app) {
    SDL_App* application = (SDL_App*)app;

    // Ensure the application exists.
    if (application == NULL ||
        application->update == NULL ||
        application->update(application->userData) == SDL_FALSE) {
        emscripten_cancel_main_loop();
    }
}
#endif

#if !defined(SDL_APP_NO_ENTRY)
/**
 * The main entry point of the application.
 *
 * Will call the Main() function.
 *
 * @see SDL_APP_MAIN
 */
int main(int argc, char* argv[]) {
    // Get the user-defined SDL_App from their Main() function.
    SDL_App app = SDL_APP_MAIN(argc, argv);

    // Call the init callback.
    if (app.init != NULL) {
        // Check if initialization worked.
        if (app.init(app.userData) == SDL_FALSE) {
            // Skip the update loop if it didn't.
            app.update = NULL;
        }
    }

    // Start the update loop
    if (app.update != NULL) {
#if defined(EMSCRIPTEN)
        // Set up the main loop.
        emscripten_set_main_loop_arg(SDL_App_Update, &app, -1, 1);
#else
        // Continue running when update returns TRUE.
        while(app.update(app.userData) == SDL_TRUE);
#endif
    }

    // Close the SDL_App and Window
    if (app.close != NULL) {
        app.close(app.userData);
    }

    // Clear up any user data.
    if (app.userData != NULL) {
        SDL_free(app.userData);
    }

    // Return an error state if update was nullified.
    return (app.update == NULL) ? 1 : 0;
}
#endif // SDL_APP_NO_ENTRY

#if defined(__cplusplus)
}
#endif

#endif // SDL_APP_IMPLEMENTATION_ONCE
#endif // SDL_APP_IMPLEMENTATION
