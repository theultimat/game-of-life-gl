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

    // We want the world to wrap around so configure the border appropriately.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GOL_CHECK_GL();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GOL_CHECK_GL();

    GOL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GOL_CHECK_GL();
}


void gol_create_state(GolState *state, int width, int height, const GLubyte *initial_state)
{
    memset(state, 0, sizeof *state);

    {
        char *vss = gol_load_string("../shaders/vs.glsl");
        char *fss = gol_load_string("../shaders/update.glsl");

        gol_create_program(&state->program, vss, fss);

        free(fss);
        free(vss);
    }

    glGenFramebuffers(2, state->fbos);
    glGenTextures(2, state->textures);

    GOL_CHECK_GL();

    init_framebuffer(state->fbos[0], state->textures[0], width, height, initial_state);
    init_framebuffer(state->fbos[1], state->textures[1], width, height, NULL);
}

void gol_destroy_state(GolState *state)
{
    gol_destroy_program(&state->program);
    glDeleteFramebuffers(2, state->fbos);
    glDeleteTextures(2, state->textures);

    GOL_CHECK_GL();
}
