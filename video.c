// video.c

#include "video.h"

u16 *videoBuffer = (u16*) 0x6000000;

// Sets the pixel at row r, column c to color
void setPixel(u16 r, u16 c, u8 color)
{
	u16 pixel = r*240 + c;
	u16 offset = pixel >> 1;
	u16 ts = videoBuffer[offset];
	if (c & 1)
	{
		// odd column
		videoBuffer[offset] = (color << 8) | (ts & 0x00FF);
	}
	else
	{
		// even column
		videoBuffer[offset] = (ts & 0x00FF) | color;
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
