// main.c

#include "main.h"

#define MINE_NUM	10	

int indicatorRow = 5;
int indicatorCol = 5;
int oldIndicatorRow = 5;
int oldIndicatorCol = 5;

enum {BLACKIDX, REDIDX, BLUEIDX, GREENIDX};
u16 colors[] = {BLACK, RED, BLUE, GREEN};

int
main()
{

	int numcolors = sizeof(colors) / sizeof(colors[0]);
	for (int i = 0; i < numcolors; i++)
	{
		PALETTE[i] = colors[i];
	}

	REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;
	placeMines();
	setMineCounts();
	drawDiscoveredField();
	flipPage();
	drawDiscoveredField();
	while(1)
	{
			keyHandle();
			waitForVBlank();
			flipPage();
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
void placeMines() {
	int minesPlaced = 0;
	while (minesPlaced < MINE_NUM) {
		int x = qran_range(0,10);
		int y = qran_range(0,10);
		if (field[x][y] != 9) {
			field[x][y] = 9;
			minesPlaced++;
		}	
	}
	printField();
	DEBUG_PRINT("\n=======================================\n");
}

void setMineCounts() {
	for (int x=0; x<10; x++)
	{
		for (int y=0; y<10; y++)
		{
			int checkLeft = 1;
			int checkRight = 1;
			int checkTop = 1;
			int checkBottom = 1;
			if (field[x][y] != 9) 
			{	
				int cell = 0;
				if (y==0)	checkLeft = 0;
				else if (y==9)	checkRight = 0;
				if (x==0)	checkTop = 0;
				else if (x==9)	checkBottom = 0;

				if (checkLeft) 
				{
					cell += checkCell(field[x][y-1]);
					if (checkTop)
						cell += checkCell(field[x-1][y-1]);
					if (checkBottom)
						cell += checkCell(field[x+1][y-1]);
				}
				if (checkRight)
				{
					cell += checkCell(field[x][y+1]);
					if (checkTop)
						cell += checkCell(field[x-1][y+1]);
					if (checkBottom)
						cell += checkCell(field[x+1][y+1]);
				}
				if (checkTop)
					cell += checkCell(field[x-1][y]);
				if (checkBottom)
					cell += checkCell(field[x+1][y]);

				field[x][y] = cell;
			}
		}
	}
	printField();
}

int checkCell(int cell) {
	if (cell == 9) {
		return 1;
	}
	return 0;
}

void printField() {
	for (int x=0; x<10; x++)
	{
		for (int y=0; y<10; y++)
		{
			if (field[x][y] == 9)
				DEBUG_PRINT("X, ");
			else
				DEBUG_PRINTF("%d, ", field[x][y]);
		}
		DEBUG_PRINT("\n");
	}
}
