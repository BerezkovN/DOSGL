/*/// vga.h
///	This header is resposible for initializing VGA MODE X(unchained mode)
///	Also it includes functions for page flipping and plotting pixels
/// There are no functions to work with usual 13h mode
/// And also palette support is removed. If you need it, look at the article below
///
/// Code was taken and cleaned up from David Brackeen's article:
/// http://www.brackeen.com/vga/unchain.html#3
/// 
/// Writen by Nikita Berezkov
//*/

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode or usually reffred as 13h mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SC_INDEX            0x03c4    /* VGA sequence controller */
#define SC_DATA             0x03c5
#define CRTC_INDEX          0x03d4    /* VGA CRT controller */
#define CRTC_DATA           0x03d5
#define INPUT_STATUS_1      0x03da

#define MAP_MASK            0x02      /* Sequence controller registers */
#define ALL_PLANES          0xff02
#define MEMORY_MODE         0x04

#define HIGH_ADDRESS        0x0C      /* CRT controller registers */
#define LOW_ADDRESS         0x0D
#define UNDERLINE_LOCATION  0x14
#define MODE_CONTROL        0x17

#define DISPLAY_ENABLE      0x01      /* VGA input status bits */
#define VRETRACE            0x08

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define SCREEN_SIZE         (word)(SCREEN_WIDTH*SCREEN_HEIGHT)

#define VERTICAL_RETRACE              /* comment out this line for more accurate timing */
typedef unsigned char	byte;
typedef unsigned int	word;
typedef unsigned long	dword;

extern byte* VGA;        /* this points to video memory. */

extern word visual_page;
extern word active_page;

void set_mode(byte mode);
void set_unchained_mode(void);
void page_flip(word* page1, word* page2);
void setpix(word page, int x, int y, byte c);