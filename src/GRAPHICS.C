#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <i86.h>


/* My things */
#include "datatypes.h"
#include "objects.h"
#include "asm.h"

/* VGA & Video Things */
#define MODE_VGA 0x13
#define MODE_TEXT 0x03

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define YOFFSET(N) ((N<<8) + (N<<6))

/*Special Colors*/
#define TRANSPARENT 0x00

#define ERROR 1

unsigned char *VGA_SCRN = (unsigned char *)0xA0000;
extern char startup_issues;
extern short x_scroll;
extern short y_scroll;

void set_mode(unsigned char mode){
	union REGS regs;

	regs.h.ah = 0x00;
	regs.h.al = mode;
	int386(0x10, &regs, &regs);
	return;
}

/* General Functions */
unsigned char *init_screen(){
	//Initialize the video buffer
	unsigned char *vga_buffer;
	vga_buffer = (unsigned char *)malloc(SCREEN_SIZE);
	if(vga_buffer == NULL){	//Fail if out of memory
		printf("Error! Could not allocate memory to the buffer!\n");
		startup_issues++;
		return NULL;
	}
	set_mode(MODE_VGA); //Set MODE to 0x13
	return vga_buffer;
}

unsigned char *create_screen_buffer(){
	unsigned char *vga_buffer;
	vga_buffer = (unsigned char *)malloc(SCREEN_SIZE);
	if(vga_buffer == NULL){
		printf("Error! Could not allocate memory to the additonal buffer!\n");
		startup_issues++;
		return NULL;
	}
	return vga_buffer;
}

void buffer_copy(unsigned char *from, unsigned char *to){
	memcpy(to, from, SCREEN_SIZE);
}

void screen_copy(unsigned char *vga_buffer){
	memcpy(VGA_SCRN, vga_buffer, 64000);
}


void clear_screen(unsigned char bg_color, unsigned char *vga_buffer){
	memset(vga_buffer, bg_color, 64000);
}


/* Graphics Draw */

//Has transparency (mostly for sprites and tiles with transparency)
void draw_tile(unsigned char *vga_buffer, tileset tc, unsigned char tile, short x, short y){
	unsigned short read_i, write_x, storage_x, size, write_y, storage_y;
	write_x = write_y = 0;
	storage_x = storage_y = 0;
	size = ((tc.height)*(tc.width));
	for(read_i = (tile*size); read_i < ((tile+1)*size); read_i++){
		if(write_x && !(write_x %= tc.width)){
			write_y++;
		}
		storage_x = (write_x++)+x;
		storage_y = write_y+y;
		if(!(tc.pointer[read_i])){ //If pixel is transparent, give up
			continue;
		}
		if(((storage_x) >= 0) && ((storage_x) < SCREEN_WIDTH)){	//X bounds check
			if(((storage_x) >= 0) && ((storage_y) < SCREEN_HEIGHT)){ //Y bounds check
				vga_buffer[(((storage_x))+(YOFFSET(storage_y)))] = tc.pointer[read_i];
			}
		}
	}
	return;
}

//Doesn't care for transparency (just ment to be quick)
/*
void draw_tile_fast(unsigned char *vga_buffer, tileset tc, unsigned short tile, short x, short y){
	unsigned short read_y, size_offset = tc.width, pointer_offset=0, size;
	size = (tc.height*tc.width);
	if(x < 0){
		size_offset = x + tc.width;
		pointer_offset = (x*-1);
		x=0;
	}
	if(x > (SCREEN_WIDTH-tc.width)){
		size_offset = (SCREEN_WIDTH-x) ;
		
	}
	if(x < (-tc.width) || x >= (SCREEN_WIDTH)){
		return;
	}
	for(read_y=0; read_y < tc.height; read_y++){
		//printf("%x", read_y + tc.pointer);
		memcpy((vga_buffer+(x+(YOFFSET(y+read_y)))), tc.pointer+(read_y*tc.width)+pointer_offset+(tile*size), (size_offset));
	}
	return;
}
*/
void draw_tile_fast(unsigned char *vga_buffer, tileset tc, unsigned short tile, short x, short y){
	short y_starting_pos=y, x_starting_pos=x, y_ending_pos, x_ending_pos, read_y, y_offset = 0, x_offset = 0, size;
	size = (tc.height*tc.width);
	y_ending_pos = tc.height; //Initial Yend
	x_ending_pos = tc.width; //Initial Xend
	if(y > SCREEN_HEIGHT || x > SCREEN_WIDTH || y < 0-(tc.height) || x < 0-(tc.width)){ //Fully offscreen
		return;
	}
	if(y < 0){ //If offscreen to up
		y_ending_pos += y;
		y_offset = tc.height - y_ending_pos;
		y_starting_pos = 0;
	}
	if(x < 0){ //If offscreen to left
		x_ending_pos += x;
		x_offset = tc.width - x_ending_pos;
		x_starting_pos = 0;
	}
	if((y+y_ending_pos) > SCREEN_HEIGHT){ // If offscreen to down
		y_ending_pos -= ((y+y_ending_pos) - SCREEN_HEIGHT);
	}
	if((x+x_ending_pos) > SCREEN_WIDTH){ // If offscreen to right
		x_ending_pos -= ((x+x_ending_pos) - SCREEN_WIDTH);
	}
	for(read_y = 0; read_y < y_ending_pos; read_y++){
		memcpy(vga_buffer+(x_starting_pos+YOFFSET(y_starting_pos+read_y)), tc.pointer+x_offset+((read_y+y_offset)*tc.width)+(tile*size), x_ending_pos);
	}
}

void draw_tile_fast_trans(unsigned char *vga_buffer, tileset tc, unsigned short tile, short x, short y){
	short y_starting_pos=y, x_starting_pos=x, y_ending_pos, x_ending_pos, read_y, y_offset = 0, x_offset = 0, size;
	size = (tc.height*tc.width);
	y_ending_pos = tc.height; //Initial Yend
	x_ending_pos = tc.width; //Initial Xend
	if(y > SCREEN_HEIGHT || x > SCREEN_WIDTH || y < 0-(tc.height) || x < 0-(tc.width)){ //Fully offscreen
		return;
	}
	if(y < 0){ //If offscreen to up
		y_ending_pos += y;
		y_offset = tc.height - y_ending_pos;
		y_starting_pos = 0;
	}
	if(x < 0){ //If offscreen to left
		x_ending_pos += x;
		x_offset = tc.width - x_ending_pos;
		x_starting_pos = 0;
	}
	if((y+y_ending_pos) > SCREEN_HEIGHT){ // If offscreen to down
		y_ending_pos -= ((y+y_ending_pos) - SCREEN_HEIGHT);
	}
	if((x+x_ending_pos) > SCREEN_WIDTH){ // If offscreen to right
		x_ending_pos -= ((x+x_ending_pos) - SCREEN_WIDTH);
	}
	for(read_y = 0; read_y < y_ending_pos; read_y++){
		memcpy_transparent(vga_buffer+(x_starting_pos+YOFFSET(y_starting_pos+read_y)), tc.pointer+x_offset+((read_y+y_offset)*tc.width)+(tile*size), x_ending_pos);

	}
}


void draw_road(unsigned char *vga_buffer, tileset road, short horz_position, unsigned char anim_looper){
	int i, line_drawn = 255, z=0, dz, dzz = 1;
	short starting_horz_position = horz_position;
	char pal_flag = 0;
	if(!(anim_looper % 4) && anim_looper){
		pal_flag == 1;
	}
	for(i = 100; i > 0; i--){
		dz += dzz;
		z -= dz;
		if(!((i-anim_looper) % 8) && anim_looper){
			pal_flag ^= 1;
		}
		line_drawn-=2;
		if(horz_position > 0){
			horz_position++;
		}if(horz_position < 0){
			horz_position--;
		}
		if(1){
			if(pal_flag){
				memcpy(vga_buffer+YOFFSET(200-(i)), road.pointer + horz_position + 128 + ((255-line_drawn) * road.width), SCREEN_WIDTH);
			}else{
				memcpy(vga_buffer+YOFFSET(200-(i)), road.pointer + horz_position + 128 + ((255-line_drawn) * road.width) + 510*255, SCREEN_WIDTH);
			}
		}
	}
	return;
}

/*
void draw_road(unsigned char *vga_buffer, tileset road, char position, char pal_select){
	int i;
	for(i = 0; i < 100; i++){
		memcpy((vga_buffer+SCREEN_SIZE-YOFFSET(i)), (road.pointer+(SCREEN_SIZE-(i*SCREEN_WIDTH))), SCREEN_WIDTH);	
	}
	return;
}
*/

void draw_sky(unsigned char *vga_buffer, tileset sky, unsigned short horizon_height){
	unsigned char i;
	for(i = 0; i < 200; i++){
		memset(vga_buffer+((SCREEN_WIDTH*i)), sky.pointer[i], SCREEN_WIDTH);
	}
	return;
}

void draw_sprites(unsigned char *vga_buffer, sprites *sp){
	unsigned char i;
	for(i = 0; i < MAX_NUMBER_OF_SPRITES; i++){
		if(sp->slot[i].status){
			draw_tile_fast_trans(vga_buffer, *sp->slot[i].g, sp->slot[i].tile, sp->slot[i].x-x_scroll, sp->slot[i].y-y_scroll);
		}
	}
	return;
}

/* Color palette */
void set_color_palette(unsigned char index, unsigned short r, unsigned short g, unsigned short b){
	outp(0x03c8, index);
	outp(0x03c9, r);
	outp(0x03c9, g);
	outp(0x03c9, b);
	return;
}

void palette_cycle(unsigned char start, unsigned char end){
	unsigned char i, r_store, g_store, b_store, r, g, b;
}



