// dma.c

#include "dma.h"
// Does a DMA copy from src to dst of size count
void dmacpy(void *dst, const void *src, u16 count)
{
	DMA[3].cnt = 0;
	DMA[3].src = src;
	DMA[3].dst = dst;
	DMA[3].cnt = count | DMA_ON;
}
