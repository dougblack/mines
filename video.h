// video.h

#include "dma.h"

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3           3
#define MODE4			4
#define BG2_ENABLE      (1<<10)
#define BUFFER0	(u16 *) 0x6000000
#define BUFFER1	(u16 *) 0x600A000
#define BUFFER1FLAG		(1<<4)
#define PALETTE	((u16 *)	0x5000000)


#define RGB(r,g,b)		((r) | ((g)<<5) | ((b)<<10))
#define RED             RGB(31,0,0)
#define BLUE            RGB(0,0,31)
#define GREEN           RGB(0,31,0)
#define YELLOW          RGB(31,31,0)
#define MAGENTA         RGB(31,0,31)
#define CYAN            RGB(0,31,31)
#define WHITE           RGB(31,31,31)
#define LIGHTBLUE       RGB(0,0,10)
#define LIGHTGREEN      RGB(0,10,0)
#define LIGHTRED        RGB(31,10,0)
#define BLACK			RGB(0,0,0) 

#define SCANLINECOUNTER (*(unsigned short *) 0x4000006)

extern u16 *videoBuffer;
extern int field[20][20];

void setPixel(u16 r, u16 c, u8 color);
void drawRect4(int r, int c, int width, int height, u8 color);
void drawImage4(int r, int c, int width, int height, const u8* image);
void fillScreen4(u8 index);