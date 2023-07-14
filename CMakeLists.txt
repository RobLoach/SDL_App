cmake_minimum_required(VERSION 3.11)
project(SDL_App
    DESCRIPTION "SDL_App: Application wrapper for SDL."
    HOMEPAGE_URL "https://github.com/RobLoach/SDL_App"
    VERSION 1.0.0
    LANGUAGES C
)

# raylib-app
add_library(SDL_App INTERFACE)
target_include_directories(SDL_App INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

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
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        set(CTEST_CUSTOM_TESTS_IGNORE
            pkg-config--static
        )
       add_subdirectory(test)
    endif()
endif()
