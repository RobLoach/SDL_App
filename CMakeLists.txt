cmake_minimum_required(VERSION 3.11)
project(SDL_app
    DESCRIPTION "SDL_app: Application wrapper for SDL."
    HOMEPAGE_URL "https://github.com/robloach/SDL_app"
    VERSION 0.0.1
    LANGUAGES C
)

# raylib-app
add_library(SDL_app INTERFACE)
target_include_directories(SDL_app INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(SDL_APP_IS_MAIN TRUE)
else()
    set(SDL_APP_IS_MAIN FALSE)
endif()
option(SDL_APP_BUILD_EXAMPLES "Examples" ${SDL_APP_IS_MAIN})

# Examples
if (SDL_APP_BUILD_EXAMPLES)
    add_subdirectory(examples)

    # Testing
    #include(CTest)
    #enable_testing()
    #if (BUILD_TESTING)
        # set(CTEST_CUSTOM_TESTS_IGNORE
        #     pkg-config--static
        # )
    #    add_subdirectory(test)
    #endif()
endif()