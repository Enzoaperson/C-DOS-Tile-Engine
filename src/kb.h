#ifndef KB
#define KB

#define KEY_ESC     0x01
#define KEY_W       0x11
#define KEY_A       0x1E
#define KEY_S       0x1F
#define KEY_D       0x20
#define KEY_SHIFT   0x2A
#define KEY_CTRL    0x1D
#define KEY_SPACE   0x39
#define KEY_I	    0x17
#define KEY_J       0x24
#define KEY_K	    0x25
#define KEY_L       0x26

extern unsigned char keys[256];

void __interrupt kbc_irq_handler();

#endif
