#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#include "../datatype.h"
#include "../camera.h"
#include "../kb.h"

#define FALLING_ACCELERATION 1

extern unsigned char keys[256];
extern char frame;
extern map mp;

void player(sprite *sp){
	short tile_x = ((sp->x+(sp->g->width/2))/16), tile_y = ((sp->y+sp->g->height)/16);
	//Init itself if it already hasn't
	if(sp->gss_1 == -1){ 
		sp->gss_1 = 0;
	}
	if(sp->gss_2 == -1){
		sp->gss_2 = 128;
	}
	if(sp->gss_2 > 128){
		sp->gss_2 = 0;
	}
	//Actual code
	if(keys[KEY_J] == 1){
		sp->x--;
		cam_hoz_scroll(-1);
	}
	if(keys[KEY_L] == 1){
		sp->x++;
		if(sp->x >= 320/2){
			cam_hoz_scroll(1);
		}
	}
	
	if(!frame){
		sp->tile ^= 1;
	}
	if(!mp.data_pointer[tile_x + tile_y*(mp.hoz_screens*(320/16))]){
		if(!(frame % 12) && sp->gss_1 < 4){
			sp->gss_1++;
		}
		sp->y+=sp->gss_1;
		cam_ver_scroll(sp->gss_1);
	}else{
		sp->gss_1 = 1;
	}
	return;
}
