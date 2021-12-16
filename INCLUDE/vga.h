/*/// vga.h
///	Цей файл має функції для ініціалізації Mode X та звичайного текстового режиму
//*/

#define VIDEO_INT           0x10      /* BIOS преривання */
#define SET_MODE            0x00      /* BIOS значення для задавання режиму. */
#define VGA_256_COLOR_MODE  0x13      /* 13h mode. */
#define TEXT_MODE           0x03      /* Текстовий режим. */

#define SC_INDEX            0x03c4    
#define SC_DATA             0x03c5
#define CRTC_INDEX          0x03d4    
#define CRTC_DATA           0x03d5
#define INPUT_STATUS_1      0x03da

#define MAP_MASK            0x02      
#define ALL_PLANES          0xff02
#define MEMORY_MODE         0x04

#define HIGH_ADDRESS        0x0C      
#define LOW_ADDRESS         0x0D
#define UNDERLINE_LOCATION  0x14
#define MODE_CONTROL        0x17

#define DISPLAY_ENABLE      0x01     
#define VRETRACE            0x08

#define SCREEN_WIDTH        320       /* Ширина екрану */
#define SCREEN_HEIGHT       200       /* Висота екрану */
#define SCREEN_SIZE         (word)(SCREEN_WIDTH*SCREEN_HEIGHT)

#define VERTICAL_RETRACE              
typedef unsigned char	byte;
typedef unsigned int	word;
typedef unsigned long	dword;

extern byte* VGA;        /* Цей вказівник посилається на частину пам'яті яку зчитує відеокарта */

extern word visual_page;
extern word active_page;

void set_mode(byte mode);
void set_unchained_mode(void);
void page_flip(word* page1, word* page2);
void setpix(word page, int x, int y, byte c);