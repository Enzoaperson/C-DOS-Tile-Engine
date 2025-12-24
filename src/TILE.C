/*
 * File: TILE.C
 * 
 * Sort of just an extention on GRAPHICS.C
 * Includes functions on opening tilesets
 * and closeing them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <i86.h>

/* My Things */
#include "datatype.h"
#include "graphics.h"


extern char startup_issues;


/*Handling Tile graphics*/
tileset open_tileset(char *path){
	unsigned short i;
	//Open file
	tileset ts;
	ts.file = fopen(path, "rb");

	if(ts.file == NULL){
		printf("Error! couldn't open %s!\n", path);
		startup_issues++;
		return ts;
	}
	//Get length
	fseek(ts.file, 0, SEEK_END);
	ts.length = ftell(ts.file);
	fseek(ts.file, 0, SEEK_SET);

	//Getting Color Palette into RAM

	//Setting dimentions
	ts.width = fgetc(ts.file) << 8;
	ts.width += fgetc(ts.file);
	ts.height = fgetc(ts.file) << 8;
	ts.height += fgetc(ts.file);

	//Allocating RAM for the Tileset
	ts.pointer = (unsigned char *)malloc(ts.length - (ts.amount_colors * 3) - 3);
	if(ts.pointer == NULL){
		printf("Error! couldn't allocate memory for tileset %s", path);
		startup_issues++;
		return ts;
	}
	//Reading Tileset into RAM
	fread(ts.pointer, 1, (ts.length), ts.file);
	fclose(ts.file);
	return ts;
}

void close_tileset(tileset ts){
	//fclose(ts.file);
	free(ts.pointer);
	return;
}


