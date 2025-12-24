#ifndef DATATYPES
#define DATATYPES

#include <stdio.h>
#define MAX_NUMBER_OF_SPRITES 10

/*Tileset: The Graphics for Sprites and Maps*/
typedef struct{
	FILE *file;
	unsigned char *pointer, *palette_pointer;
	unsigned short height, width;
	unsigned int length;
	unsigned char palette_offset, amount_colors;
}tileset;

/*Sprite: A moveable thingy with a behavoir*/
typedef struct {
	tileset *g;
	char status, tile; // 1 = Active
	short x, y, whatami, gss_1, gss_2;
}sprite;

/*Sprites: A table of sprites. Easily iteriable*/
typedef struct {
	sprite slot[MAX_NUMBER_OF_SPRITES];
	char next_free;
}sprites;


/*Map: Basically a level*/
typedef struct{
	FILE *file;
	tileset *tile_pointer;
	unsigned short length;
	unsigned char *data_pointer; //This will become a short later
	unsigned char ver_screens, hoz_screens;
}map;

#endif
