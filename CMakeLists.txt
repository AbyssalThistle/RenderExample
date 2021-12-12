cmake_minimum_required(VERSION 3.15)
project(template)

set(CMAKE_C_STANDARD 11)

# setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/raylib)

add_compile_options("-Wall" "-Wextra" "-ggdb")

file(GLOB_RECURSE SRC
	src/*.c
	vendor/clogs/src/*.c
	vendor/raylibExtras/rlExtrasC/*.c
)

add_executable(game ${SRC})
target_include_directories(game
	PUBLIC src
	PUBLIC vendor/vector/src
	PUBLIC vendor/clogs/src
	PUBLIC vendor/raylib/external
	PUBLIC vendor/raylibExtras/rlExtrasC
)

target_link_libraries(game PRIVATE raylib)
