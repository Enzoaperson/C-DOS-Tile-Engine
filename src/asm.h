#ifndef ASM
#define ASM


int memcpy_transparent(void *to, const void *from, char length);
#pragma aux memcpy_transparent =  \
				  "mov ecx,0xFFFFFFFF" \
				  "draw_loop: inc ecx" \
				  "cmp cl,dl" \
				  "je draw_loop_end" \
				  "mov dh,[ebx+ecx]" \
				  "test dh,dh" \
				  "jz draw_loop" \
				  "mov [eax+ecx],dh" \
				  "jmp draw_loop" \
			          "draw_loop_end: nop" \
				  modify [ebx ecx edx dh esi] parm [eax] [ebx] [dl] value [ecx];

/*
int memcpy_transparent(void *to, const void *from, short length);
#pragma aux memcpy_transparent = \
				 "	     mov ecx, 0xFFFF   " \
				 "draw_loop: inc ecx           " \
				 "           cmp cx,bx         " \
				 "           je draw_loop_end  " \
				 "           mov dh,[esi+ecx]  " \
                                 "           test dh,dh        " \
				 "           jz draw_loop      " \
				 "           mov [edi+ecx],dh  " \
				 "           jmp draw_loop     " \
				 "draw_loop_end:               " \
				modify [esi edi ecx bx dh] parm [edi] [esi] [bx] value [EDI];
*/
#endif
