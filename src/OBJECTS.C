#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <i86.h>

#include "datatype.h"
#include "creatures/creatures.h"
#define SUCCESS 0
#define FAILED 1

extern char frame;

sprite init_sprite(){
	sprite s;
	s.status = 0;
	s.x = 0;
	s.y = 0;
	s.tile = 0;
	s.whatami = 0;
	s.gss_1 = -1;
	s.gss_2 = -1;
	return s;
}

sprites init_sprite_table(){
	char i;
	sprites sp;
	for(i = 0; i < MAX_NUMBER_OF_SPRITES; i++){
		sp.slot[i] = init_sprite();
	}
	sp.next_free = 0;
	return sp;
}

char create_sprite(sprites *sp, tileset *tc_pointer, short x, short y, short id){
	//Check if slot is empty
	unsigned char i;
	if(sp->slot[sp->next_free].status){
		return FAILED;
	}
	sp->slot[sp->next_free].g = tc_pointer;
	sp->slot[sp->next_free].x = x;
	sp->slot[sp->next_free].y = y;
	sp->slot[sp->next_free].whatami = id;
	sp->slot[sp->next_free].status = 1;
	sp->next_free++;
	return SUCCESS;
}

//Moves 1 pixel to the right every 3 frames 
void test_obj_run(sprite *sp){
	if(!(frame % 3)){
		sp->x++;
	}
	if(sp->x > 320){
		sp->x = 0;
	}
	return;
}

void run_objects(sprites *sp){
	char i;
	for(i = 0; i < MAX_NUMBER_OF_SPRITES; i++){
		if(sp->slot[i].status){ //if status is enabled
			switch (sp->slot[i].whatami){
				case 0x00: 
					break;
				case 0x01:
					test_obj_run(&sp->slot[i]);
					break;
				case 0x02:
					fish(&sp->slot[i]);
					break;
				case OBJ_PLAYER:
					player(&sp->slot[i]);
					break;


			}
		}
	}
	return;
}

