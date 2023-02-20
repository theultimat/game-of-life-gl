#ifndef GOL_CANVAS_H
#define GOL_CANVAS_H

#include <glad/gl.h>


// Full screen triangle used to invoke the state update per-tick.
typedef struct
{
    GLuint vao;
    GLuint vbo;
} GolCanvas;


void gol_create_canvas(GolCanvas *canvas);
void gol_destroy_canvas(GolCanvas *canvas);

#endif
