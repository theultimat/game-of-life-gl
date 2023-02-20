#ifndef GOL_WINDOW_H
#define GOL_WINDOW_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>


typedef struct
{
    GLFWwindow *window;
    int width;
    int height;
} GolWindow;


void gol_create_window(GolWindow *window, int width, int height);
void gol_destroy_window(GolWindow *window);

#endif
