#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "io.h"


char *gol_load_string(const char *path)
{
    FILE *f = fopen(path, "rb");
    GOL_ASSERT_MSG(f, "Failed to open file: '%s'.", path);

    fseek(f, 0, SEEK_END);
    size_t n = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *str = malloc(n + 1);
    GOL_ASSERT(str);

    size_t m = fread(str, 1, n, f);
    fclose(f);

    GOL_ASSERT_MSG(n == m, "Only read %zu/%zu bytes from file '%s'.", m, n, path);

    str[n] = '\0';

    return str;
}
