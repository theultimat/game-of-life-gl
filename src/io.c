#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "io.h"


static char *load_padded_bytes(const char *path, int postpad, char fill)
{
    FILE *f = fopen(path, "rb");
    GOL_ASSERT_MSG(f, "Failed to open file: '%s'.", path);

    fseek(f, 0, SEEK_END);
    size_t n = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data = malloc(n + postpad);
    GOL_ASSERT(data);

    size_t m = fread(data, 1, n, f);
    fclose(f);

    GOL_ASSERT_MSG(n == m, "Only read %zu/%zu bytes from file '%s'.", m, n, path);

    for (int i = 0; i < postpad; ++i)
        data[n + i] = fill;

    return data;
}


char *gol_load_string(const char *path)
{
    return load_padded_bytes(path, 1, '\0');
}

unsigned char *gol_load_bytes(const char *path)
{
    return (unsigned char *)load_padded_bytes(path, 0, 0);
}
