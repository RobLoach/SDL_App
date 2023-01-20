
    find_package(SDL2 REQUIRED)
    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)

# raylib-nuklear-test
add_executable(SDL_app_test SDL_app_test.c)
target_compile_options(SDL_app_test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(SDL_app_test PUBLIC
    ${SDL2_LIBRARIES}
    SDL_app
)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME SDL_app_test COMMAND SDL_app_test)