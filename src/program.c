#include <string.h>

#include "assert.h"
#include "program.h"


static GLuint compile_shader(GLenum type, const char *src)
{
    GLuint shader = glCreateShader(type);
    GOL_CHECK_GL();

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GOL_CHECK_GL();

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar log[1024];
        glGetShaderInfoLog(shader, sizeof log, NULL, log);

        GOL_ASSERT_MSG(success, "Shader compilation failed: %s", log);
    }

    return shader;
}


void gol_create_program(GolProgram *prog, const char *vss, const char *fss)
{
    memset(prog, 0, sizeof *prog);

    prog->name = glCreateProgram();
    GOL_CHECK_GL();

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vss);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fss);

    glAttachShader(prog->name, vs);
    glAttachShader(prog->name, fs);

    GOL_CHECK_GL();

    glLinkProgram(prog->name);

    GOL_CHECK_GL();

    GLint success;
    glGetProgramiv(prog->name, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLchar log[1024];
        glGetProgramInfoLog(prog->name, sizeof log, NULL, log);

        GOL_ASSERT_MSG(success, "Program link failed: %s", log);
    }

    glDetachShader(prog->name, fs);
    glDetachShader(prog->name, vs);

    GOL_CHECK_GL();

    glDeleteShader(fs);
    glDeleteShader(vs);

    GOL_CHECK_GL();
}

void gol_destroy_program(GolProgram *prog)
{
    if (prog->name)
    {
        glDeleteProgram(prog->name);
        GOL_CHECK_GL();
    }
}
