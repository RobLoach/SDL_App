
find_package(SDL2 REQUIRED)
set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)

# SDL_App_Test
add_executable(SDL_App_Test SDL_App_Test.c)
target_compile_options(SDL_App_Test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(SDL_App_Test PUBLIC
    ${SDL2_LIBRARIES}
    SDL_App
)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME SDL_App_Test COMMAND SDL_App_Test)
