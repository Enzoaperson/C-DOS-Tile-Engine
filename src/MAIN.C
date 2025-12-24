#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <i86.h>

/* My Things */
#include "creatures/creatures.h"
#include "datatype.h"
#include "graphics.h"
#include "objects.h"
#include "camera.h"
#include "tile.h"
#include "asm.h"
#include "map.h"
#include "kb.h"

#define SCROLL_SPEED 2

char startup_issues = 0;
char frame = 0;

map mp;
void (__interrupt *prev_int_09)(); 

extern short x_scroll, y_scroll;

int main(){
	unsigned char gu8_i, quit = 0;
	unsigned char *screen, *tile_buffer;
	short x_scroll_old = 1, y_scroll_old = 0;
	unsigned int test = 10;
	tileset enzo, sky, fish, font, smb, s1nums;
	tileset dummy;
	//map mp;
	sprites sp;
	
	//Set KB as an Interupt
	prev_int_09 = _dos_getvect(0x09);
	_dos_setvect(0x09, kbc_irq_handler);

	//Seed a randomness
	srand(time(NULL));
	//Clear Sprite table
	sp = init_sprite_table();

	//Loading graphics
	sky = open_tileset("data/skyop.dat");
	enzo = open_tileset("data/enzo.dat");
	fish = open_tileset("data/fish.dat");
	font = open_tileset("data/font.dat");
	smb = open_tileset("data/smb.dat");
	s1nums = open_tileset("data/s1nums.dat");
	//dummy = open_tileset("na.dat"); // Should produce an error (because it doesn't exist!)

	//Making some sprites
	create_sprite(&sp, &enzo, (SCREEN_WIDTH/2)-(enzo.width/2), (SCREEN_HEIGHT/2)+(enzo.height/2), OBJ_PLAYER);
	//create_sprite(&sp, &fish, 64, 64, OBJ_FISH);
	//create_sprite(&sp, &fish, 15, 45, OBJ_FISH);
	//create_sprite(&sp, &font, 100, 100, 0x00);

	//Loading Map
	mp = open_map("data/map.dat", &smb);

	screen = create_screen_buffer(); 
	tile_buffer = create_screen_buffer();

	if(startup_issues){
		printf("There were problems while loading graphics and sprites...\n");
		printf("Press 'c' to continue if you dare.\n");
		printf("Press any other key to quit\n");
		if(getch() != 'c'){
			return 1;
		}
	}
	//Start graphics 
	set_mode(MODE_VGA);
	clear_screen(0x00, screen);
	clear_screen(0x00, tile_buffer);

	while(!quit){
		//Game Logic
		if(keys[KEY_ESC] == 1){
			quit++;
		}
		if(keys[KEY_A] == 1){
			cam_hoz_scroll(-2);
		}
		if(keys[KEY_D] == 1){
			cam_hoz_scroll(2);
		}
		if(keys[KEY_S] == 1){
			cam_ver_scroll(2);
		}
		if(keys[KEY_W] == 1){
			cam_ver_scroll(-2);
		}
		if(keys[KEY_SPACE] == 1){
			if(gu8_i){
				srand(time(NULL));
				create_sprite(&sp, &fish, rand()%320, rand()%200, OBJ_FISH);
				gu8_i = 0;
			}
		}
		if(keys[KEY_SPACE] == 0){
			gu8_i = 1;
		}
		run_objects(&sp);
		
		//Screen Draw
		if(x_scroll != x_scroll_old || y_scroll != y_scroll_old){
			draw_sky(tile_buffer, sky, 0);
			draw_map(tile_buffer, mp, x_scroll, y_scroll);
			x_scroll_old = x_scroll;
			y_scroll_old = y_scroll;
		}
		buffer_copy(tile_buffer, screen);
		draw_sprites(screen, &sp);
		//memcpy_transparent(screen, (enzo.pointer+60), 120);
		//Pause for Vblanking
		while ((inp(INPUT_STATUS) & VRETRACE));
		while (!(inp(INPUT_STATUS) & VRETRACE));
		//Copy VGA buffer to VGA display
		screen_copy(screen);
		frame++;
		frame %= 70;
	
	}
	set_mode(MODE_TEXT);
	//Return interrupt
	_dos_setvect(0x09, prev_int_09);
	printf("%p\n", enzo.pointer+test);
	printf("%x\n", memcpy_transparent(screen, enzo.pointer+test, 2));
	close_tileset(sky);
	close_tileset(enzo);
	close_tileset(fish);
	printf("Bye bye!\n");
       	return 0;	
	
}
