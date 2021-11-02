#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#include <mem.h>

#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE 0x03

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

#define INPUT_STATUS 0x03DA
#define MISC_OUTPUT 0x3C2
#define SC_INDEX 0x3C4
#define SC_DATA 0x3C5
#define GC_INDEX 0x03CE
#define GC_DATA 0x03CF
#define CRTC_INDEX 0x03D4
#define CRTC_DATA 0x03D51

#define DISPLAY_ENABLE 0x01
#define VRETRACE 0x08

/* Sequence controller registers */
#define MAP_MASK 0x02
#define ALL_PLANES 0xFF02
#define MEMORY_MODE 0x04

/* CRT controller registers */
#define HIGH_ADDRESS 0x0C
#define LOW_ADDRESS 0x0D
#define UNDERLINE_LOCATION 0x14
#define MODE_CONTROL 0x17

unsigned int vgabuffer_length;

typedef unsigned char byte;
typedef int WORD;

byte far* VGA = (byte far*)0xA0000000L;

byte* vgabuffer;

WORD vga_page[4];

#define SETPIX(x,y,c) *(vgabuffer+(x)+(y)*SCREEN_WIDTH)=c
#define GETPIX(x,y) *(vgabuffer+(x)+(y)*SCREEN_WIDTH)

void init_vgabuffer() {
    vgabuffer_length = (unsigned int)SCREEN_HEIGHT * (unsigned int)SCREEN_WIDTH;
    vgabuffer = (byte*)malloc(vgabuffer_length);
    memset(vgabuffer, 0, vgabuffer_length);


    if (vgabuffer == 0)
        cout << "VGA buffer failed to initialize!!!\n";
}

void swap_vgabuffer() {
    for (int ind = 0; ind < vgabuffer_length; ind++)
    {
        *(VGA + ind) = *(vgabuffer + ind);
    }
    memset(vgabuffer, 0, vgabuffer_length);
}

void set_mode(byte mode)
{
    union REGS regs;
    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

void set_mode_y()
{
    set_mode(VGA_256_COLOR_MODE);
    vga_page[0] = 0;
    vga_page[1] = (SCREEN_WIDTH * SCREEN_HEIGHT) / 4 * 1;
    vga_page[2] = (SCREEN_WIDTH * SCREEN_HEIGHT) / 4 * 2;
    vga_page[3] = (SCREEN_WIDTH * SCREEN_HEIGHT) / 4 * 3;
    /* disable chain 4 */
    outportb(SC_INDEX, MEMORY_MODE);
    outportb(SC_DATA, 0x06);
    /* disable doubleWORD mode */
    outportb(CRTC_INDEX, UNDERLINE_LOCATION);
    outportb(CRTC_DATA, 0x00);
    /* disable WORD mode */
    outportb(CRTC_INDEX, MODE_CONTROL);
    outportb(CRTC_DATA, 0xE3);
    /* clear all VGA mem */
    outportb(SC_INDEX, MAP_MASK);
    outportb(SC_DATA, 0xFF);
    /* write 2^16 nulls */
    memset(VGA + 0x0000, 0, 0x8000); /* 0x10000 / 2 = 0x8000 */
    memset(VGA + 0x8000, 0, 0x8000); /* 0x10000 / 2 = 0x8000 */
}

void setpix(WORD page, int x, int y, byte c)
{
    outportb(SC_INDEX, MAP_MASK);
    outportb(SC_DATA, 1 << (x & 3));
    VGA[page + ((SCREEN_WIDTH * y) >> 2) + (x >> 2)] = c; /* x/4 is equal to x>>2 */
}

void page_flip(WORD* page1, WORD* page2)
{
    WORD temp;
    WORD high_address, low_address;

    temp = *page1;
    *page1 = *page2;
    *page2 = temp;

    high_address = HIGH_ADDRESS | (*page1 & 0xFF00);
    low_address = LOW_ADDRESS | (*page1 << 8);
    /*
      instead of:
      outportb( CRTC_INDEX, HIGH_ADDRESS );
      outportb( CRTC_DATA, (*page1 & 0xFF00) >> 8 );
      do this:
      high_address = HIGH_ADDRESS | (*page1 & 0xFF00);
      outport( CRTC_INDEX, high_address );
     */
    while (inp(INPUT_STATUS) & DISPLAY_ENABLE);
    outport(CRTC_INDEX, high_address);
    outport(CRTC_INDEX, low_address);
    while (!(inp(INPUT_STATUS) & VRETRACE));
}

void copy2page(byte* s, WORD page, int h)
{
    int x, y;
    byte c;
    for (y = 0; y < h; y++) {
        for (x = 0; x < SCREEN_WIDTH; ++x) {
            c = s[y * SCREEN_WIDTH + x];
            setpix(page, x, y, c);
        }
    }
}