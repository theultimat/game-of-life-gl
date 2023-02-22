#ifndef GOL_IO_H
#define GOL_IO_H


typedef struct
{
    int width;
    int height;
    unsigned char *data;
} GolPbm;


char *gol_load_string(const char *path);
unsigned char *gol_load_bytes(const char *path);

void gol_load_pbm(GolPbm *pbm, const char *path);

#endif
