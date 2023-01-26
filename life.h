#ifndef _LIFE_H
#define _LIFE_H

#include <stdbool.h>

struct Game {
    unsigned long generation;
    int len;
    bool *grid;
    bool *past_grid;
    int width;
    int height;
};

#endif
