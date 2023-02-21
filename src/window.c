#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "window.h"


static void glfw_error_callback(int error, const char *message)
{
    fprintf(stderr, "GLFW ERROR %d: %s\n", error, message);
    abort();
}

void gol_create_window(GolWindow *window, int width, int height)
{
    memset(window, 0, sizeof *window);

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    GOL_ASSERT(width > 0);
    GOL_ASSERT(height > 0);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    window->window = glfwCreateWindow(width, height, "Game of Life", NULL, NULL);

    glfwSetWindowUserPointer(window->window, window);
    glfwMakeContextCurrent(window->window);

    int glad_okay = gladLoadGL(glfwGetProcAddress);
    GOL_ASSERT_MSG(glad_okay, "Failed to initialise GLAD.");

    glfwGetFramebufferSize(window->window, &window->width, &window->height);
    glViewport(0, 0, window->width, window->height);
    GOL_CHECK_GL();
}

void gol_destroy_window(GolWindow *window)
{
    if (window->window)
    {
        glfwDestroyWindow(window->window);
        glfwTerminate();
    }
}
