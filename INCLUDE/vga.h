#include <dos.h>
#include <mem.h>

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */


#define SC_INDEX            0x03c4    /* VGA sequence controller */
#define SC_DATA             0x03c5
#define PALETTE_INDEX       0x03c8    /* VGA digital-to-analog converter */
#define PALETTE_DATA        0x03c9
#define GC_INDEX            0x03ce    /* VGA graphics controller */
#define GC_DATA             0x03cf
#define CRTC_INDEX          0x03d4    /* VGA CRT controller */
#define CRTC_DATA           0x03d5
#define INPUT_STATUS_1      0x03da

#define MAP_MASK            0x02      /* Sequence controller registers */
#define ALL_PLANES          0xff02
#define MEMORY_MODE         0x04

#define LATCHES_ON          0x0008    /* Graphics controller registers */
#define LATCHES_OFF         0xff08

#define HIGH_ADDRESS        0x0C      /* CRT controller registers */
#define LOW_ADDRESS         0x0D
#define UNDERLINE_LOCATION  0x14
#define MODE_CONTROL        0x17

#define DISPLAY_ENABLE      0x01      /* VGA input status bits */
#define VRETRACE            0x08

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define SCREEN_SIZE         (word)(SCREEN_WIDTH*SCREEN_HEIGHT)
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

//#define VERTICAL_RETRACE              /* comment out this line for more
                                        //accurate timing */
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

byte* VGA = (byte*)0xA0000000L;        /* this points to video memory. */
byte* double_buffer;

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

void plot_pixel(int x, int y, byte color)
{
    outp(SC_INDEX, MAP_MASK);          /* select plane */
    outp(SC_DATA, 1 << (x & 3));

    VGA[(y << 6) + (y << 4) + (x >> 2)] = color;
}