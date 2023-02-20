#ifndef GOL_PROGRAM_H
#define GOL_PROGRAM_H

#include <glad/gl.h>


typedef struct
{
    GLuint name;
} GolProgram;


void gol_create_program(GolProgram *prog, const char *vss, const char *fss);
void gol_destroy_program(GolProgram *prog);

#endif
