#ifndef ASM
#define ASM


int memcpy_transparent(void *to, const void *from, int length);
#pragma aux memcpy_transparent =  \
				  "mov ecx,0xFFFFFFFF" \
				  "draw_loop: inc ecx" \
				  "cmp ecx,esi" \
				  "je draw_loop_end" \
				  "mov dh,[ebx+ecx]" \
				  "test dh,dh" \
				  "jz draw_loop" \
				  "mov [eax+ecx],dh" \
				  "jmp draw_loop" \
			          "draw_loop_end: nop" \
				  modify [ebx ecx edx esi] parm [eax] [ebx] [esi] value [ecx];


#endif
