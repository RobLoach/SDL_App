/**********************************************************************************************
*
*   SDL_App v0.0.1 - Application wrapper for SDL.
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

typedef struct SDL_App SDL_App;

/**
 * Application data that is used to manage the window.
 */
struct SDL_App {
    /**
     * The initiatialization callback for the application.
     *
     * InitWindow() will be called prior to init() being called.
     *
     * @param app The SDL_App information for the currently running application.
     */
    void (*init)(SDL_App* app);

    /**
     * The update callback to update and draw the application.
     *
     * @param app The SDL_App information for the currently running application.
     */
    void (*update)(SDL_App* app);

    /**
     * The close callback used to deinitialize all application data.
     *
     * CloseWindow() is called after the callback is run.
     *
     * @param app The SDL_App information for the currently running application.
     */
    void (*close)(SDL_App* app);

    /**
     * When set to TRUE, will stop running the update() callback and close the application.
     *
     * @see CloseApp()
     */
    SDL_bool shouldClose;

    /**
     * The exit status to return after the application has been run.
     *
     * Set this to 1 if you'd like to report an error when exiting.
     */
    int exitStatus;

    /**
     * Custom user data that is passed through the application callbacks.
     *
     * This is helpful if you have context that needs to be passed through all the callbacks.
     */
    void* userData;
};

/**
 * Tells the application that it should close.
 *
 * @param app The application that should be closed.
 */
void SDL_AppClose(SDL_App* app);

#if defined(__cplusplus)
}
#endif

#endif  // SDL_APP_H_

#ifdef SDL_APP_IMPLEMENTATION
#ifndef SDL_APP_IMPLEMENTATION_ONCE
#define SDL_APP_IMPLEMENTATION_ONCE

// raylib.h
#ifndef SDL_APP_SDL_H
#define SDL_APP_SDL_H <SDL2/SDL.h>
#endif
#include SDL_APP_SDL_H

// emscripten.h
#if defined(EM_JS)
#ifndef SDL_APP_EMSCRIPTEN_H
#define SDL_APP_EMSCRIPTEN_H "emscripten.h"
#endif
#include SDL_APP_EMSCRIPTEN_H
#endif

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
 * @see SDL_App
 * @param argc The length of the argument vector.
 * @param argv The array of arguments.
 * @example examples/core_basic_window.c
 *
 * @return The SDL_App description for your Application.
 */
extern SDL_App Main(int argc, char* argv[]);
#endif

/**
 * Informs the application that it should close.
 *
 * @param app The application that should close.
 */
void SDL_AppClose(SDL_App* app) {
    app->shouldClose = SDL_TRUE;
}

#if defined(EM_JS)
/**
 * The update callback for web.
 */
void SDL_App_Update(void* app) {
    SDL_App* application = (SDL_App*)app;
    if (application == NULL) {
        return;
    }

    if (application->shouldClose == SDL_TRUE) {
        // Tell emscripten that it should stop doing the main loop.
        emscripten_cancel_main_loop();
        return;
    }

    // Call the update function.
    if (application->update != NULL) {
        application->update(application);
    }
}
#endif

#if !defined(SDL_APP_NO_ENTRY)
/**
 * The main entry point for raylib-app.
 */
int main(int argc, char* argv[]) {
    // Get the user-defined SDL_App from their Main() function.
    SDL_App app = Main(argc, argv);

    // Allow exiting early if desired.
    if (app.shouldClose) {
        return app.exitStatus;
    }

    // Call the init callback.
    if (app.init != NULL) {
        app.init(&app);
    }

    // Start the update loop
    if (app.update != NULL) {
#if defined(EM_JS)
        emscripten_set_main_loop_arg(SDL_App_Update, &app, 0, 0);
#else
        // Stop running if the Window or SDL_App have been told to close.
        while (app.shouldClose != SDL_TRUE) {
            app.update(&app);
        }
#endif
    }

    // Close the SDL_App and Window
    if (app.close != NULL) {
        app.close(&app);
    }

    // Clean up any other user data.
    if (app.userData != NULL) {
        SDL_free(app.userData);
        app.userData = NULL;
    }

    return app.exitStatus;
}
#endif // SDL_APP_NO_ENTRY

#if defined(__cplusplus)
}
#endif

#endif // SDL_APP_IMPLEMENTATION_ONCE
#endif // SDL_APP_IMPLEMENTATION
