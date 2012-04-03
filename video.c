// video.c

#include "video.h"

u16 *videoBuffer = (u16*) 0x6000000;

// Sets the pixel at row r, column c to color
void setPixel(u16 row, u16 col, u8 index)
{
	u16* hiddenBuffer = buffer_hidden();
	int whichPixel = OFFSET(row, col, 240);
	int whichShort = whichPixel/2;
	if(col & 1)
	{
		hiddenBuffer[whichShort] = (hiddenBuffer[whichShort] & 0x00FF) | (index << 8);
	}
	else
	{
		hiddenBuffer[whichShort] = (hiddenBuffer[whichShort] & 0xFF00) | (index);
	}
}

// Draws a rectange starting at top left corner at (r,c) with
// the specified width and height and color
void drawRect4(int r, int c, int width, int height, u8 index)
{

	u16* hiddenBuffer = buffer_hidden();
	volatile u16 color = index | (index << 8);
	for (int i=0; i<height; i++) {
		DMA[3].cnt = 0;
		DMA[3].src = &color;
		DMA[3].dst = hiddenBuffer + (((r+i)*240 + c) >> 1);
		DMA[3].cnt = (width>>1) | DMA_ON | DMA_SOURCE_FIXED;
	}
}

// Draws the image using DMA
void drawImage4(int r, int c, int width, int height, const u8* image)
{
	u16* hiddenBuffer = buffer_hidden();
	int rowWidth = width;
	for (int i = 0; i < height; i++) {
		const u8* row = image+i*rowWidth;
		u16 *dest = hiddenBuffer+((r+i)*240+c);
		dmacpy(dest, row, rowWidth);
	}
}

void fillScreen4(u8 index)
{
	u16* hiddenBuffer = buffer_hidden();
	volatile u16 color = index | (index << 8);
	DMA[3].cnt = 0;
	DMA[3].src = &color;
	DMA[3].dst = hiddenBuffer;
	DMA[3].cnt = 19200 | DMA_ON | DMA_SOURCE_FIXED;
}

void drawDiscoveredField()
{
	drawFieldBorders();	
	for (int x=0; x<10; x++)
	{
		for (int y=0; y<10; y++)
		{
			drawCell(x, y, field[x][y]);	
		}
	}
}
void drawFieldBorders()
{

	for (int i=10; i<=150; i++)
	{
		setPixel(i,50, 2);
		setPixel(i,190, 2);
	}
	for (int i=50; i<=190; i++)
	{
		setPixel(10, i, 2);
		setPixel(150, i, 2);
	}
	for (int x=1; x<10; x++)
	{
		for (int i=10; i<=150; i++) {
			setPixel(i, 50+(x*14), 2);
		}
		for (int i=50; i<=190; i++) {
			setPixel(10+(x*14),i,2);
		}
	}
}

void drawIndicator(indicator ind) {
	
	int x = 10+ind.r*14;
	int y = 50+ind.c*14;	
	for (int i=1;i<14;i++)
	{
		setPixel(x+1, y+i,1);
		setPixel(x+13, y+i,1);
		setPixel(x+i, y+1, 1);
		setPixel(x+i, y+13, 1); 
	}
}

void clearIndicator(indicator ind) {

	int x = 10+ind.old_r*14;
	int y = 50+ind.old_c*14;	
	for (int i=1;i<14;i++)
	{
		setPixel(x+1, y+i,0);
		setPixel(x+13, y+i,0);
		setPixel(x+i, y+1, 0);
		setPixel(x+i, y+13, 0); 
	}
}
u16* 
buffer_hidden() 
{
	if (REG_DISPCTL & BUFFER1FLAG)
	{
		return BUFFER0;
	} 
	else 
	{
		return BUFFER1;
	}
}

void drawCell(int r, int c, int cellValue)
{

	switch (cellValue) {
		case 0:
	}

}
