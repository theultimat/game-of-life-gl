#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>

#include "assert.h"


void gol_assert_fail(const char *exp, const char *file, int line, const char *func)
{
    fprintf(stderr, "ASSERT FAILED: '%s'\n%s:%d %s\n", exp, file, line, func);
    abort();
}

void gol_assert_fail_msg(const char *exp, const char *file, int line, const char *func,
    const char *fmt, ...)
{
    fprintf(stderr, "ASSERT FAILED: '%s': ", exp);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n%s:%d %s\n", file, line, func);
    abort();
}

void gol_check_gl(const char *file, int line, const char *func)
{
    GLenum error = glGetError();

    if (error == GL_NO_ERROR)
        return;

    fprintf(stderr, "OPENGL ERROR: 0x%x\n%s:%d %s\n", error, file, line, func);
    abort();
}
