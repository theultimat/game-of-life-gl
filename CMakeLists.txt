cmake_minimum_required(VERSION 3.13)
project(gol LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL 3.3 REQUIRED)

add_executable(gol
    src/main.c
)

target_compile_options(gol PRIVATE
    -Wall
    -Werror
    -Wextra
    -O3
    -g
)

target_link_libraries(gol PRIVATE
    OpenGL::GL
)
