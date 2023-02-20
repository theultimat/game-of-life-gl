#include <string.h>

#include "assert.h"
#include "canvas.h"


void gol_create_canvas(GolCanvas *canvas)
{
    memset(canvas, 0, sizeof *canvas);

    glGenVertexArrays(1, &canvas->vao);
    glGenBuffers(1, &canvas->vbo);

    GOL_CHECK_GL();

    GLfloat vertices[] =
    {
        // X     Y
        -1.0f, -1.0f,
        -1.0f,  3.0f,
         3.0f, -1.0f,
    };

    glBindVertexArray(canvas->vao);
    glBindBuffer(GL_ARRAY_BUFFER, canvas->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    GOL_CHECK_GL();

    // We only need position, coordinates within the canvas can be determined
    // in the shader using gl_FragCoord.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof *vertices * 2, (const void *)0);
    glEnableVertexAttribArray(0);

    GOL_CHECK_GL();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GOL_CHECK_GL();
}

void gol_destroy_canvas(GolCanvas *canvas)
{
    if (canvas->vao)
    {
        glDeleteVertexArrays(1, &canvas->vao);
        GOL_CHECK_GL();
    }

    if (canvas->vbo)
    {
        glDeleteBuffers(1, &canvas->vbo);
        GOL_CHECK_GL();
    }
}
