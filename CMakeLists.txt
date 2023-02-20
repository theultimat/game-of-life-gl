cmake_minimum_required(VERSION 3.13)
project(gol LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL 3.3 REQUIRED)

set(GLAD_OUT_DIR ${CMAKE_BINARY_DIR}/gladsources)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/glad/cmake)
add_subdirectory(3rdparty/glfw)

glad_add_library(glad REPRODUCIBLE API gl:core=3.3 LOCATION ${GLAD_OUT_DIR})

add_executable(gol
    src/assert.c
    src/main.c
    src/window.c
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
    glfw
    glad
    OpenGL::GL
)
