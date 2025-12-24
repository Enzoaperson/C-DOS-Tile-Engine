#ifndef MAP
#define MAP

#include "datatype.h"

map open_map(char *path, tileset *tile_pointer);

void close_map(map mp);

void draw_map(unsigned char *screen, map mp, short x_scroll, short y_scroll);

#endif
