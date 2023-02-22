#ifndef GOL_STATE_H
#define GOL_STATE_H

#include <glad/gl.h>

#include "canvas.h"
#include "program.h"


// Maintains everything required to update the board state.
typedef struct
{
    GolProgram program;
    GLuint fbos[2];
    GLuint textures[2];
    GLuint active;
    GLint u_prev_state;
    GLint u_state_size;
    GLuint width;
    GLuint height;
} GolState;


void gol_create_state(GolState *state, int width, int height, float zoom, const GLubyte *initial_state);
void gol_destroy_state(GolState *state);

void gol_tick_state(GolState *state, const GolCanvas *canvas);


int gol_calculate_state_width(int width, float zoom);
int gol_calculate_state_height(int height, float zoom);

#endif
