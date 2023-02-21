#ifndef GOL_STATE_H
#define GOL_STATE_H

#include <glad/gl.h>

#include "program.h"


// Maintains everything required to update the board state.
typedef struct
{
    GolProgram program;
    GLuint fbos[2];
    GLuint textures[2];
    GLuint active;
} GolState;


void gol_create_state(GolState *state, int width, int height);
void gol_destroy_state(GolState *state);

#endif
