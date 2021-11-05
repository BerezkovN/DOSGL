#include <dos.h>
#include <mem.h>

#include "vga.h"

byte* VGA = (byte*)0xA0000000L;        /* this points to video memory. */
byte* double_buffer;

word visual_page;
word active_page;

/**************************************************************************
*  set_mode                                                              *
*     Sets the video mode.                                               *
**************************************************************************/

void set_mode(byte mode)
{
    union REGS regs;

    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

/**************************************************************************
*  set_unchained_mode                                                    *
*    resets VGA mode 0x13 to unchained mode to access all 256K of memory *
**************************************************************************/

void set_unchained_mode(void)
{
    set_mode(VGA_256_COLOR_MODE);
    word i;
    dword* ptr = (dword*)VGA;            /* used for faster screen clearing */

    outp(SC_INDEX, MEMORY_MODE);       /* turn off chain-4 mode */
    outp(SC_DATA, 0x06);

    outpw(SC_INDEX, ALL_PLANES);        /* set map mask to all 4 planes */

    for (i = 0; i < 0x4000; i++)               /* clear all 256K of memory */
        *ptr++ = 0;

    outp(CRTC_INDEX, UNDERLINE_LOCATION);/* turn off long mode */
    outp(CRTC_DATA, 0x00);

    outp(CRTC_INDEX, MODE_CONTROL);      /* turn on byte mode */
    outp(CRTC_DATA, 0xe3);
}

/**************************************************************************
*  page_flip                                                             *
*    switches the pages at the appropriate time and waits for the        *
*    vertical retrace.                                                   *
**************************************************************************/

void page_flip(word* page1, word* page2)
{
    word high_address, low_address;
    word temp;

    temp = *page1;
    *page1 = *page2;
    *page2 = temp;

    high_address = HIGH_ADDRESS | (*page1 & 0xff00);
    low_address = LOW_ADDRESS | (*page1 << 8);

#ifdef VERTICAL_RETRACE
    while ((inp(INPUT_STATUS_1) & DISPLAY_ENABLE));
#endif
    outpw(CRTC_INDEX, high_address);
    outpw(CRTC_INDEX, low_address);
#ifdef VERTICAL_RETRACE
    while (!(inp(INPUT_STATUS_1) & VRETRACE));
#endif
}
/**************************************************************************
*  show_buffer                                                           *
*    displays a memory buffer on the screen                              *
**************************************************************************/

void show_buffer(byte* buffer)
{
#ifdef VERTICAL_RETRACE
    while ((inp(INPUT_STATUS_1) & VRETRACE));
    while (!(inp(INPUT_STATUS_1) & VRETRACE));
#endif
    memcpy(VGA, buffer, SCREEN_SIZE);
}

/**************************************************************************
*  set_palette                                                           *
*    Sets all 256 colors of the palette.                                 *
**************************************************************************/

void set_palette(byte* palette)
{
    int i;

    outp(PALETTE_INDEX, 0);              /* tell the VGA that palette data
                                            is coming. */
    for (i = 0; i < 256 * 3; i++)
        outp(PALETTE_DATA, palette[i]);    /* write the data */
}

/**************************************************************************
*  plot_pixel                                                            *
*    Plots a pixel in unchained mode                                     *
**************************************************************************/

void setpix(word page, int x, int y, byte c)
{
    outportb(SC_INDEX, MAP_MASK);
    outportb(SC_DATA, 1 << (x & 3));

    VGA[page + (((word)SCREEN_WIDTH * y) >> 2) + (x >> 2)] = c; /* x/4 is equal to x>>2 */
}
