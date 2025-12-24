#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#include "datatype.h"
#include "graphics.h"

extern char startup_issues;

map open_map(char *path, tileset *tile_pointer){
	map mp;
	mp.file = fopen(path, "rb");
	if(mp.file == NULL){
		printf("Error! couldn't open %s map!\n", path);
		startup_issues++;
		return mp;
	}
	fseek(mp.file, 0, SEEK_END);
	mp.length = ftell(mp.file);
	fseek(mp.file, 0, SEEK_SET);

	//Setting dimentions
	mp.hoz_screens = fgetc(mp.file);
	mp.ver_screens = fgetc(mp.file);

	//Setting graphics
	mp.tile_pointer = tile_pointer;
	
	//Allocating RAM for Map
	mp.data_pointer = (unsigned char *)malloc(mp.length);
	if(mp.data_pointer == NULL){
		printf("Error! Couldn't allocate memory for the map: %s", path);
		startup_issues++;
		return mp;
	}
	fread(mp.data_pointer, 1, mp.length, mp.file);
	fclose(mp.file);
	return mp;
}

void close_map(map mp){
	//fclose(mp.file);
	free(mp.data_pointer);
	return;
}


void draw_map(unsigned char *screen, map mp, short x_scroll, short y_scroll){
	unsigned short i2, i, tile_height, tile_width, what_am_i_drawing;
	tile_height = SCREEN_HEIGHT/(mp.tile_pointer->height)+2;
	tile_width = SCREEN_WIDTH/(mp.tile_pointer->width)+1;
	for(i2 = 0; i2 < (tile_height); i2++){
		for(i = 0; i < tile_width; i++){
			what_am_i_drawing = ((x_scroll/16) + i) + ((i2 + (y_scroll/16))*(mp.hoz_screens*(20)));
			if(mp.data_pointer[what_am_i_drawing]){
				draw_tile_fast(screen, *mp.tile_pointer, mp.data_pointer[what_am_i_drawing], (i*(mp.tile_pointer->width))-(x_scroll%(mp.tile_pointer->width)), (i2*(mp.tile_pointer->height))-(y_scroll%(mp.tile_pointer->height)));
			}
		}
	}
}
