#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#include "datatypes.h"

short x_scroll = 0, y_scroll = 0;
extern map mp;

char cam_hoz_scroll(short x){
	if(((x_scroll + x) >= 0 ) && (x_scroll+x) < ((mp.hoz_screens-1)*320)){
		x_scroll += x;
		return 0;
	}else{
		return 1;
	}
	
}

char cam_ver_scroll(short y){
	if(((y_scroll + y) >= 0 ) && (y_scroll+y) < ((mp.ver_screens-1)*200)){
		y_scroll += y;
		return 0;
	}else{
		return 1;
	}
	
}

char cam_set_pos(short x, short y){
	if((y >= 0 ) && y < ((mp.ver_screens-1)*200)){
		y_scroll = y;
	}
	if((x >= 0 ) && x < ((mp.hoz_screens-1)*320)){
		x_scroll = x;
	}
	return 0;
}


