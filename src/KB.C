#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#define IRQ_PORT    0x20
#define KBC_IRQ     0x09
#define KBC_PORT    0x60
#define KBC_BREAK   0x80

unsigned char keys[256];

extern void (__interrupt *prev_int_09)();

/* Figure out how to make this IRQ based for real*/
void __interrupt kbc_irq_handler(){
	unsigned char code, val =1;
	code = inp(KBC_PORT);
	if(code & KBC_BREAK){
		val = 0;
		code -= KBC_BREAK;
	}else{
		val = 1;
	}
	keys[code] = val;
	_chain_intr(prev_int_09);
}
