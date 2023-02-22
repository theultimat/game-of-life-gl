#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "assert.h"
#include "canvas.h"
#include "io.h"
#include "program.h"
#include "state.h"
#include "window.h"


#define GOL_TICKS_PER_SECOND (4.0)
#define GOL_SECONDS_PER_TICK (1.0 / GOL_TICKS_PER_SECOND)


static void parse_args(int argc, char *argv[], const char **path, float *zoom)
{
    // Set default values.
    *path = NULL;
    *zoom = 1.0f;

    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--zoom"))
        {
            ++i;
            GOL_ASSERT_MSG(i < argc, "--zoom expects an argument");

            *zoom = strtof(argv[i], NULL);
            GOL_ASSERT_MSG(*zoom >= 1.0f, "--zoom must be >= 1");
        }
        else
        {
            *path = argv[i];
        }
    }
}

static GLubyte *create_initial_state(const char *path, int width, int height, float zoom)
{
    width = ceilf(width / zoom);
    height = ceilf(height / zoom);

    GLubyte *data = malloc(width * height);
    GOL_ASSERT(data);

    memset(data, 0, width * height);

    // If we don't pass a path or the option is --random use a random initial
    // state, otherwise load the PBM file and center it in the window.
    if (!path || !strcmp(path, "--random"))
    {
        srand(time(NULL));

        for (int i = 0; i < width * height; ++i)
            data[i] = (rand() % 2) * 255;
    }
    else
    {
        GolPbm pbm;
        gol_load_pbm(&pbm, path);

        GOL_ASSERT(pbm.width <= width && pbm.height <= height);

        int x_offset = (width - pbm.width) / 2;
        int y_offset = (height - pbm.height) / 2;

        for (int y = 0; y < pbm.height; ++y)
        {
            const GLubyte *src = pbm.data + y * pbm.width;
            GLubyte *dst = data + (y + y_offset) * width + x_offset;

            memcpy(dst, src, pbm.width);
        }

        free(pbm.data);
    }

    return data;
}

static void draw(
    const GolWindow *window,
    const GolCanvas *canvas,
    const GolState *state,
    const GolProgram *prog,
    GLint u_state_texture,
    GLint u_height,
    GLint u_zoom,
    float zoom
)
{
    // Note - we don't need to do a clear as we're always going to be writing
    // to every pixel.

    glViewport(0, 0, window->width, window->height);

    GOL_CHECK_GL();

    glUseProgram(prog->name);
    glUniform1i(u_state_texture, state->active);
    glUniform1i(u_height, window->height);
    glUniform1f(u_zoom, zoom);

    GOL_CHECK_GL();

    gol_draw_canvas(canvas);
}


int main(int argc, char *argv[])
{
    const char *pbm_path;
    float zoom;
    parse_args(argc, argv, &pbm_path, &zoom);

    printf("%f\n", zoom);

    GolWindow window;
    gol_create_window(&window, 1280, 720);

    GolCanvas canvas;
    gol_create_canvas(&canvas);

    GolProgram program;
    GLint u_state_texture;
    GLint u_height;
    GLint u_zoom;

    {
        char *vss = gol_load_string("../shaders/vs.glsl");
        char *fss = gol_load_string("../shaders/fs.glsl");

        gol_create_program(&program, vss, fss);

        free(fss);
        free(vss);

        u_state_texture = glGetUniformLocation(program.name, "u_StateTexture");
        GOL_ASSERT(u_state_texture >= 0);

        u_height = glGetUniformLocation(program.name, "u_Height");
        GOL_ASSERT(u_height >= 0);

        u_zoom = glGetUniformLocation(program.name, "u_Zoom");
        GOL_ASSERT(u_zoom >= 0);
    }

    GolState state;
    {
        GLubyte *init = create_initial_state(pbm_path, window.width, window.height, zoom);

        gol_create_state(&state, window.width, window.height, zoom, init);

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
            gol_tick_state(&state, &canvas);
            latency_time -= GOL_SECONDS_PER_TICK;
        }

        draw(&window, &canvas, &state, &program, u_state_texture, u_height, u_zoom, zoom);
        glfwSwapBuffers(window.window);
    }

    gol_destroy_state(&state);
    gol_destroy_program(&program);
    gol_destroy_canvas(&canvas);
    gol_destroy_window(&window);

    return 0;
}
