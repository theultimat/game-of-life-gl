cmake_minimum_required(VERSION 3.13)
project(gol LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL 3.3 REQUIRED)

set(GLAD_OUT_DIR ${CMAKE_BINARY_DIR}/gladsources)

add_subdirectory(3rdparty/glad/cmake)

glad_add_library(glad REPRODUCIBLE API gl:core=3.3 LOCATION ${GLAD_OUT_DIR})

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

target_include_directories(gol PRIVATE
    ${GLAD_OUT_DIR}/include
)

target_link_libraries(gol PRIVATE
    glad
    OpenGL::GL
)
