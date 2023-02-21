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
    for (int y = 0; y < height / 2; ++y)
    {
        GLubyte *row = data + y * width;

        for (int x = 0; x < width / 2; ++x)
            row[x] = 255;
    }

    return data;
}

static void draw(
    const GolWindow *window,
    const GolCanvas *canvas,
    const GolState *state,
    const GolProgram *prog,
    GLint u_state_texture)
{
    // Note - we don't need to do a clear as we're always going to be writing
    // to every pixel.

    glViewport(0, 0, window->width, window->height);

    GOL_CHECK_GL();

    glUseProgram(prog->name);
    glUniform1i(u_state_texture, state->active);

    GOL_CHECK_GL();

    gol_draw_canvas(canvas);
}


int main(void)
{
    GolWindow window;
    gol_create_window(&window, 1280, 720);

    GolCanvas canvas;
    gol_create_canvas(&canvas);

    GolProgram program;
    GLint u_state_texture;

    {
        char *vss = gol_load_string("../shaders/vs.glsl");
        char *fss = gol_load_string("../shaders/fs.glsl");

        gol_create_program(&program, vss, fss);

        free(fss);
        free(vss);

        u_state_texture = glGetUniformLocation(program.name, "u_StateTexture");
        GOL_ASSERT(u_state_texture >= 0);
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

        draw(&window, &canvas, &state, &program, u_state_texture);
        glfwSwapBuffers(window.window);
    }

    gol_destroy_state(&state);
    gol_destroy_program(&program);
    gol_destroy_canvas(&canvas);
    gol_destroy_window(&window);

    return 0;
}
