#include <dos.h>
#include <mem.h>

#include "vga.h"

byte* VGA = (byte*)0xA0000000L;        /* Цей вказівник посилається на частину пам'яті яку зчитує відеокарта */

word visual_page;
word active_page;

/***                                          
 *     Ініціалізує вказаний режим                                             
 */
void set_mode(byte mode)
{
    union REGS regs;

    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

/***
 *  Ініціалізує mode 13h та модифікує його для отримання mode X
 */
void set_unchained_mode(void)
{
    set_mode(VGA_256_COLOR_MODE);
    word i;
    dword* ptr = (dword*)VGA;            

    outp(SC_INDEX, MEMORY_MODE);         
    outp(SC_DATA, 0x06);

    outpw(SC_INDEX, ALL_PLANES);         

    for (i = 0; i < 0x4000; i++)         
        *ptr++ = 0;

    outp(CRTC_INDEX, UNDERLINE_LOCATION);
    outp(CRTC_DATA, 0x00);

    outp(CRTC_INDEX, MODE_CONTROL);     
    outp(CRTC_DATA, 0xe3);
}

/***
 *  Змінює вказівник відеокарти на іншу частину пам'яті, тим саме створючи ефект двійного буфера                                               
 */
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

/***
 *  Задає значення пікселя для режиму mode X                                 
 */
void setpix(word page, int x, int y, byte c)
{
    outportb(SC_INDEX, MAP_MASK);
    outportb(SC_DATA, 1 << (x & 3));

    VGA[page + (((word)SCREEN_WIDTH * y) >> 2) + (x >> 2)] = c; /* x/4 is equal to x>>2 */
}
