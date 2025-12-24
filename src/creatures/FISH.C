#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#include "../datatype.h"

extern unsigned char keys[256];
extern char frame;


void fish(sprite *sp){
	if(!(frame % 3)){
		sp->x++;
	}
	if(sp->x > 320){
		sp->x=(-(sp->g->width));
	}
	return;
}
