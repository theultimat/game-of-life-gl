#include <stdio.h>

#include "canvas.h"
#include "window.h"


#define GOL_TICKS_PER_SECOND (1.0)
#define GOL_SECONDS_PER_TICK (1.0 / GOL_TICKS_PER_SECOND)


int main(void)
{
    GolWindow window;
    gol_create_window(&window, 1280, 720);

    GolCanvas canvas;
    gol_create_canvas(&canvas);

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

        // TODO Draw state.
        glfwSwapBuffers(window.window);
    }

    gol_destroy_canvas(&canvas);
    gol_destroy_window(&window);

    return 0;
}
