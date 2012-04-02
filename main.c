// main.c

#include "main.h"

int indicatorRow = 5;
int indicatorCol = 5;
int oldIndicatorRow = 5;
int oldIndicatorCol = 5;

enum {REDIDX, BLUEIDX, GREENIDX};
u16 colors[] = {RED, BLUE, GREEN};

int
main()
{

	int numcolors = sizeof(colors) / sizeof(colors[0]);
	for (int i = 0; i < numcolors; i++)
	{
		PALETTE[i] = colors[i];
	}

	REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;
//	setupMineField();
	while(1)
	{
			keyHandle();
			waitForVBlank();
			fillScreen4(REDIDX);
	}

}

void keyHandle() {
			key_poll();		
			if (key_hit(KEY_RIGHT)) 
				indicatorMove(1,0);
			if (key_hit(KEY_LEFT))
				indicatorMove(-1,0);
			if (key_hit(KEY_UP)) 
				indicatorMove(0,1);
			if (key_hit(KEY_DOWN))
				indicatorMove(0,-1);
			if (key_hit(KEY_A)) 
				sweep();
			if (key_hit(KEY_B))
				placeFlag();
			if (key_hit(KEY_START))
				pause();
}

void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void indicatorMove(int delta_x, int delta_y) {

}

void flipPage() {

	if (REG_DISPCTL & BUFFER1FLAG)
	{
		REG_DISPCTL &= ~BUFFER1FLAG;
		videoBuffer = BUFFER1;
	}
	else
	{
		REG_DISPCTL |= BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}

}

void sweep() {}
void placeFlag() {}
void pause() {}
