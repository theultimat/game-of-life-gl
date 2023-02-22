#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


typedef enum
{
    PBM_PARSE_WIDTH,
    PBM_PARSE_HEIGHT,
    PBM_PARSE_DATA,
} PbmState;

void gol_load_pbm(GolPbm *pbm, const char *path)
{
    memset(pbm, 0, sizeof *pbm);

    char *str = gol_load_string(path);

    const char *rd_ptr = str;
    size_t n = strlen(str);

    GOL_ASSERT_MSG(n >= 8, "%s doesn't have enough bytes for a pbm header!", path);
    GOL_ASSERT_MSG(!memcmp(rd_ptr, "P1\n", 3), "%s doesn't have a valid pbm header!", path);

    rd_ptr += 3;

    unsigned char *wr_ptr = NULL;
    int bytes_read = 0;

    // After P1 we expect: width, height, data.
    PbmState state = PBM_PARSE_WIDTH;
    bool parsing_comment = false;

    while (*rd_ptr)
    {
        // Skip until end of comment.
        if (parsing_comment)
        {
            if (*rd_ptr++ == '\n')
                parsing_comment = false;

            continue;
        }

        // Check for comment start.
        if (*rd_ptr == '#')
        {
            parsing_comment = true;
            ++rd_ptr;
            continue;
        }

        // Skip whitespace.
        if (isspace(*rd_ptr))
        {
            ++rd_ptr;
            continue;
        }

        switch (state)
        {
        case PBM_PARSE_WIDTH:
            while (rd_ptr && isdigit(*rd_ptr))
            {
                pbm->width *= 10;
                pbm->width += *rd_ptr++ - '0';
            }

            state = PBM_PARSE_HEIGHT;
            break;
        case PBM_PARSE_HEIGHT:
            while (rd_ptr && isdigit(*rd_ptr))
            {
                pbm->height *= 10;
                pbm->height += *rd_ptr++ - '0';
            }

            // Once we've parsed width and height we can allocate the data
            // buffer.
            int size = pbm->width * pbm->height;
            GOL_ASSERT(size > 0);

            pbm->data = malloc(size);
            GOL_ASSERT(pbm->data);
            wr_ptr = pbm->data;

            state = PBM_PARSE_DATA;
            break;
        case PBM_PARSE_DATA:
            GOL_ASSERT_MSG(*rd_ptr == '0' || *rd_ptr == '1', "Bad pbm data: %c.", *rd_ptr);
            GOL_ASSERT(bytes_read < pbm->width * pbm->height);
            *wr_ptr++ = !(*rd_ptr++ - '0') * 255;
            bytes_read++;
            break;
        default:
            GOL_ASSERT_MSG(false, "Bad pbm parse state %d.", state);
        }
    }

    int data_size = pbm->width * pbm->height;

    GOL_ASSERT_MSG(pbm->width > 0, "Zero width in pbm %s!", path);
    GOL_ASSERT_MSG(pbm->height > 0, "Zero height in pbm %s!", path);
    GOL_ASSERT_MSG(pbm->data, "No data in pbm %s!", path);
    GOL_ASSERT_MSG(bytes_read == data_size, "Only %d/%d bytes read in pbm %s!",
        bytes_read, data_size, path);

    free(str);
}
