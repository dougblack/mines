// video.c

#include "video.h"

u16 *videoBuffer = (u16*) 0x6000000;

// Sets the pixel at row r, column c to color
void setPixel(u16 row, u16 col, u8 index)
{
	int whichPixel = OFFSET(row, col, 240);
	int whichShort = whichPixel/2;
	if(col & 1)
	{
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0x00FF) | (index << 8);
	}
	else
	{
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0xFF00) | (index);
	}
}

// Draws a rectange starting at top left corner at (r,c) with
// the specified width and height and color
void drawRect4(int r, int c, int width, int height, u8 index)
{

	volatile u16 color = index | (index << 8);
	for (int i=0; i<height; i++) {
		DMA[3].cnt = 0;
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + (((r+i)*240 + c) >> 1);
		DMA[3].cnt = (width>>1) | DMA_ON | DMA_SOURCE_FIXED;
	}
}

// Draws the image using DMA
void drawImage4(int r, int c, int width, int height, const u8* image)
{
	int rowWidth = width;
	for (int i = 0; i < height; i++) {
		const u8* row = image+i*rowWidth;
		u16 *dest = videoBuffer+((r+i)*240+c);
		dmacpy(dest, row, rowWidth);
	}
}

void fillScreen4(u8 index)
{
	volatile u16 color = index | (index << 8);
	DMA[3].cnt = 0;
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 19200 | DMA_ON | DMA_SOURCE_FIXED;
}

void drawDiscoveredField()
{
	drawFieldBorders();	

}

void drawFieldBorders()
{
	for (int i=10; i<=150; i++)
	{
		setPixel(i,50, 1);
		setPixel(i,190, 1);
	}
	for (int i=50; i<=190; i++)
	{
		setPixel(10, i, 1);
		setPixel(150, i, 1);
	}
	for (int x=1; x<10; x++)
	{
		for (int i=10; i<=150; i++) {
			setPixel(i, 50+(x*14), 1);
		}
		for (int i=50; i<=190; i++) {
			setPixel(10+(x*14),i,1);
		}
	}
}

void drawIndicator(int r, int c) {
	
	for (int i=1;i<14;i++)
	{
	}

}
