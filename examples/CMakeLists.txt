# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

set(SDL_SHARED FALSE)
set(SDL2_DISABLE_UNINSTALL TRUE)

# Dependencies
#find_package(SDL2 REQUIRED)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
endif()

# Resources
file(GLOB resources resources/*)
set(examples_resources)
list(APPEND examples_resources ${resources})
file(COPY ${examples_resources} DESTINATION "resources/")

# SDL_libretro_example
add_executable(SDL_App_Example SDL_App_Example.c)
target_link_libraries(SDL_App_Example PRIVATE
    ${SDL2_LIBRARIES}
    SDL_App
)
