#include <stddef.h>

#define NULLTERM (0)

struct Matrix_t
{
    size_t size[2];

    double** rows;
};