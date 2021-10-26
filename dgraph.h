#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE 0x03

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

typedef unsigned char byte;

byte far* VGA = (byte far*)0xA0000000L;

#define SETPIX(x,y,c) *(VGA+(x)+(y)*SCREEN_WIDTH)=c
#define GETPIX(x,y) *(VGA+(x)+(y)*SCREEN_WIDTH)

void clear_vga() {
    unsigned int length = (unsigned int)SCREEN_HEIGHT * (unsigned int)SCREEN_WIDTH;

    for (int ind = 0; ind < length; ind++)
    {
        *(VGA + ind) = 0;
    }
}

void set_mode(byte mode)
{
    union REGS regs;
    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}