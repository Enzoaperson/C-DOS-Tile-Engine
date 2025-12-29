#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#include "collision_defs.h"
#include "../datatype.h"
#include "../camera.h"
#include "../kb.h"

#define FALLING_ACCELERATION 1

extern unsigned char keys[256];
extern unsigned short y_scroll;
extern char frame;
extern map mp;

/*Function checks if it's colliding with something below you and sticks you to the top of that surface*/
char vertical_collision_check(short *y, short tile_x_left, short tile_x_right, short tile_y){
	if((!mp.data_pointer[tile_x_left + tile_y*(mp.hoz_screens*(320/16))]) && (!mp.data_pointer[tile_x_right + tile_y*(mp.hoz_screens*(320/16))])){
		return 1;
	}
	//y = ((y_scroll % 16) + tile_y*16);
	//y = 0;
	//Currently this only works with full height blocks. Will add some code for varried block heights
	return 0;
}


void player(sprite *sp){
	short tile_x_left = ((sp->x+(11))/16), tile_y = ((sp->y+sp->g->height)/16), tile_x_right = ((sp->x+(21))/16);
	//Init itself if it already hasn't
	if(sp->gss_1 == -1){ 
		sp->gss_1 = 0;
	}
	if(sp->gss_2 == -1){
		sp->gss_2 = 0;
	}
	if(sp->gss_2 > 128){
		sp->gss_2 = 0;
	}
	//Actual code
	if(keys[KEY_J] == 1){
		sp->x-=2;
		if(sp->x <= ((mp.hoz_screens-1)*320)+160){
			cam_hoz_scroll(-2);
		}
	}
	if(keys[KEY_L] == 1){
		sp->x+=2;
		if(sp->x >= 320/2){
			cam_hoz_scroll(2);
		}
	}
	
	if(!frame){
		sp->tile ^= 1;
	}

	//Falling Code
	if(vertical_collision_check(sp->y, tile_x_left, tile_x_right, tile_y)){
		if(!(frame % 12) && sp->gss_1 < 6){
			sp->gss_1++;
		}
		sp->y+=sp->gss_1;
		cam_ver_scroll(sp->gss_1);
	}else{
		sp->gss_1 = 1;
	}
	//Jumping code
	if(!(vertical_collision_check(sp->y, tile_x_left, tile_x_right, tile_y)) && (keys[KEY_I] == 1)){
		sp->gss_2 = 15;
	}
	if(sp->gss_2 > 0){
		cam_ver_scroll(-sp->gss_2);
		sp->y-=sp->gss_2;
		sp->gss_2 -= 1;
	}
	return;
}
