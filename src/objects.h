#ifndef OBJECTS
#define OBJECTS

#include "datatype.h"

#define SUCCESS 0
#define FAILED 1

sprite init_sprite();

sprites init_sprite_table(); 

char create_sprite(sprites *sp, tileset *tc_pointer, short x, short y, short id);

void run_objects(sprites *sp);

#endif
