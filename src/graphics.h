#ifndef GRAPHICS
#define GRAPHICS

#include "datatypes.h"

#define MODE_VGA 0x13
#define MODE_TEXT 0x03
#define INPUT_STATUS 0x03da
#define VRETRACE 0x08
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE 64000

extern unsigned char *VGA_SCRN;

void set_mode(unsigned char mode);

unsigned char *init_screen();

unsigned char *create_screen_buffer();

void buffer_copy(unsigned char *from, unsigned char *to);

void screen_copy(unsigned char *vga_buffer);

void clear_screen(unsigned char bg_color, unsigned char *vga_buffer);

void draw_tile(unsigned char *vga_buffer, tileset tc, unsigned short tile, short x, short y);

void draw_tile_fast(unsigned char *vga_buffer, tileset tc, unsigned short tile, short x, short y);

void draw_tile_fast_trans(unsigned char *vga_buffer, tileset tc, unsigned short tile, short x, short y);

void draw_sky(unsigned char *vga_buffer, tileset sky, unsigned short horizon_height);

void draw_sprites(unsigned char *vga_buffer, sprites *sp);

void set_color_palette(unsigned char index, unsigned short r, unsigned short g, unsigned short b);
/*
int add_inis(unsigned char *to, unsigned char *from);
#pragma aux add_inis =  "mov ecx, 0" \
			"draw_loop: mov [ebx],edx" \
			"jz draw_loop" \
			"mov [eax],edx" \
			"inc ecx" \
			"inc ebx" \
			"inc eax" \
			"cmp ecx, 30" \
			"jne draw_loop" \
			"finish_draw: nop" \
		       	modify [eax ebx ecx edx] parm [eax] [ebx] value [eax];
*/
/*
int memcpy_transparent(unsigned char *to, unsigned char *from, short length);
#pragma aux memcpy_transparent = "mov cx,-1" \
				  "draw_loop: inc cx" \
				  "cmp cx,dx" \
				  "je draw_loop_end" \
				  "mov dh,[ebx+ecx]" \
				  "test dh,dh" \
				  "jz draw_loop" \
				  "mov [eax+ecx],dh" \
				  "jmp draw_loop" \
			          "draw_loop_end: nop" \
				  modify [ebx ecx dx dl] parm [eax] [ebx] [dx];
*/
#endif
