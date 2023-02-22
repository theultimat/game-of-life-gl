#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "io.h"
#include "state.h"


static void init_framebuffer(GLuint fbo, GLuint texture, int width, int height, const GLubyte *data)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(GL_TEXTURE_2D, texture);

    GOL_CHECK_GL();

    // We only need a single channel for the data - really we only need one bit
    // for a cell being on or off, but this will do for now.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GOL_CHECK_GL();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GOL_CHECK_GL();

    GOL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GOL_CHECK_GL();
}


void gol_create_state(GolState *state, int width, int height, float zoom, const GLubyte *initial_state)
{
    width = gol_calculate_state_width(width, zoom);
    height = gol_calculate_state_height(height, zoom);

    memset(state, 0, sizeof *state);

    {
        char *vss = gol_load_string("../shaders/vs.glsl");
        char *fss = gol_load_string("../shaders/update.glsl");

        gol_create_program(&state->program, vss, fss);

        free(fss);
        free(vss);

        state->u_prev_state = glGetUniformLocation(state->program.name, "u_PreviousState");
        GOL_ASSERT(state->u_prev_state >= 0);

        state->u_state_size = glGetUniformLocation(state->program.name, "u_StateSize");
        GOL_ASSERT(state->u_state_size >= 0);
    }

    glGenFramebuffers(2, state->fbos);
    glGenTextures(2, state->textures);

    GOL_CHECK_GL();

    init_framebuffer(state->fbos[0], state->textures[0], width, height, initial_state);
    init_framebuffer(state->fbos[1], state->textures[1], width, height, NULL);

    // Bind textures to corresponding units for use with uniforms in the draw
    // shader program.
    for (int i = 0; i < 2; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, state->textures[i]);

        GOL_CHECK_GL();
    }

    state->width = width;
    state->height = height;
}

void gol_destroy_state(GolState *state)
{
    gol_destroy_program(&state->program);
    glDeleteFramebuffers(2, state->fbos);
    glDeleteTextures(2, state->textures);

    GOL_CHECK_GL();
}


void gol_tick_state(GolState *state, const GolCanvas *canvas)
{
    // Toggle between which state is the target and which is the previous -
    // simple double buffering logic.
    state->active = !state->active;

    // Match viewport size to the state size.
    glViewport(0, 0, state->width, state->height);

    GOL_CHECK_GL();

    // The update process is to draw the whole screen to an offscreen texture
    // and use the previous state on the GPU to determine which cells live.
    glUseProgram(state->program.name);
    glUniform1i(state->u_prev_state, !state->active);
    glUniform2i(state->u_state_size, state->width, state->height);

    GOL_CHECK_GL();

    glBindFramebuffer(GL_FRAMEBUFFER, state->fbos[state->active]);

    GOL_CHECK_GL();

    gol_draw_canvas(canvas);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int gol_calculate_state_width(int width, float zoom)
{
    // Each row of a texture must be aligned to a 4B boundary, so if we aren't
    // round up to the next boundary.
    width = ceilf(width / zoom);
    width = (width + 3) & ~3;

    return width;
}

int gol_calculate_state_height(int height, float zoom)
{
    return ceilf(height / zoom);
}
