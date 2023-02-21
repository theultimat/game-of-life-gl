#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "assert.h"
#include "canvas.h"
#include "io.h"
#include "program.h"
#include "state.h"
#include "window.h"


#define GOL_TICKS_PER_SECOND (1.0)
#define GOL_SECONDS_PER_TICK (1.0 / GOL_TICKS_PER_SECOND)


static GLubyte *create_initial_state(int width, int height)
{
    GLubyte *data = malloc(width * height);
    GOL_ASSERT(data);

    memset(data, 0, width * height);

    // For testing fill in a corner.
    for (int y = 0; y < 100; ++y)
    {
        GLubyte *row = data + y * width;

        for (int x = 0; x < 100; ++x)
            row[x] = 1;
    }

    return data;
}

static void draw(const GolWindow *window, const GolCanvas *canvas, const GolProgram *prog)
{
    // Note - we don't need to do a clear as we're always going to be writing
    // to every pixel.

    glViewport(0, 0, window->width, window->height);

    GOL_CHECK_GL();

    glUseProgram(prog->name);

    GOL_CHECK_GL();

    glBindVertexArray(canvas->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    GOL_CHECK_GL();
}


int main(void)
{
    GolWindow window;
    gol_create_window(&window, 1280, 720);

    GolCanvas canvas;
    gol_create_canvas(&canvas);

    GolProgram program;

    {
        char *vss = gol_load_string("../shaders/vs.glsl");
        char *fss = gol_load_string("../shaders/fs.glsl");

        gol_create_program(&program, vss, fss);

        free(fss);
        free(vss);
    }

    GolState state;
    {
        GLubyte *init = create_initial_state(window.width, window.height);

        gol_create_state(&state, window.width, window.height, init);

        free(init);
    }

    double previous_time = glfwGetTime();
    double latency_time = 0.0;

    while (!glfwWindowShouldClose(window.window))
    {
        double current_time = glfwGetTime();
        double elapsed_time = current_time - previous_time;

        previous_time = current_time;
        latency_time += elapsed_time;

        glfwPollEvents();

        while (latency_time >= GOL_SECONDS_PER_TICK)
        {
            // TODO Update state.
            latency_time -= GOL_SECONDS_PER_TICK;
        }

        draw(&window, &canvas, &program);
        glfwSwapBuffers(window.window);
    }

    gol_destroy_state(&state);
    gol_destroy_program(&program);
    gol_destroy_canvas(&canvas);
    gol_destroy_window(&window);

    return 0;
}
