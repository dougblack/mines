// main.c

#include "main.h"

#define MINE_NUM	20	

enum {BLACKIDX, WHITEIDX, REDIDX, BLUEIDX, GREENIDX, YELLOWIDX, LIGHTGRAYIDX};
u16 colors[] = {BLACK, WHITE, RED, BLUE, GREEN, YELLOW, LIGHTGRAY};
indicator ind = {5, 5, 5, 5}; 
int gameover = 0;
int cellsSwept = 0;

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
	drawFieldBorders();
	flipPage();
	drawDiscoveredField();
	drawFieldBorders();
	drawIndicator(ind);
	flipPage();
	while(!gameover)
	{
			keyHandle();
			waitForVBlank();
	}
}

void 
keyHandle() {
			key_poll();		
			int keyHandled = 0;
			if (key_hit(KEY_RIGHT)) {
				indicatorMove(1,0);
				keyHandled = 1;
			}
			if (key_hit(KEY_LEFT)) {
				indicatorMove(-1,0);
				keyHandled = 1;
			}
			if (key_hit(KEY_UP)) {
				indicatorMove(0,-1);
				keyHandled = 1;
			}
			if (key_hit(KEY_DOWN)) {
				indicatorMove(0,1);
				keyHandled = 1;
			}
			if (key_hit(KEY_A))  {
				sweep(ind.r, ind.c);
				drawDiscoveredField();
				keyHandled = 1;
			}
			if (key_hit(KEY_B)) {
				placeFlag();
				keyHandled = 1;
			}
			if (key_hit(KEY_START)) {
				pause();
				keyHandled = 1;
			}
			if (keyHandled) {
				clearIndicator(ind);
				drawIndicator(ind);	
				flipPage();
				clearIndicator(ind);
				drawDiscoveredField();
			}
}

void sweep(int r, int c) 
{
	if (field[r][c] == 9)
	{
		gameOver();
	}
	if ((discoveredField[r][c] == 0) || (discoveredField[r][c] == 7)) {
			int cell = field[r][c];
			if (cell == 0) {
				int checkLeft = 1;
				int checkRight = 1;
				int checkBottom = 1;
				int checkTop = 1;
				cell = 8;
				discoveredField[r][c] = 8;
				if (c==0)	checkLeft = 0;
				else if (c==9)	checkRight = 0;
				if (r==0)	checkTop = 0;
				else if (r==9)	checkBottom = 0;

				if (checkLeft) 
				{
					sweep(r,c-1);
					if (checkTop)
						sweep(r-1,c-1);
					if (checkBottom)
						sweep(r+1,c-1);
				}
				if (checkRight)
				{
					sweep(r,c+1);
					if (checkTop)
						sweep(r-1,c+1);
					if (checkBottom)
						sweep(r+1,c+1);
				}
				if (checkTop)
					sweep(r-1,c);
				if (checkBottom)
					sweep(r+1,c);
			}
			cellsSwept++;
			discoveredField[r][c] = cell;
			if (cellsSwept==90)
			{
				win();
			}
	}
}

void 
win()
{
	gameover = 1;
	drawRect4(10,10,10,10,1);	
}

void 
waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void 
indicatorMove(int delta_x, int delta_y) {

	int newR = ind.r + delta_y;
	int newC = ind.c + delta_x;
	if ((newR >= 0) && (newR <= 9) && (newC >= 0) && (newC <= 9)) {
		ind.old_r = ind.r;
		ind.old_c = ind.c;
		ind.r = newR;
		ind.c = newC;
	}
}

void 
flipPage() {

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

void 
placeFlag() {
	discoveredField[ind.r][ind.c] = 7;	
	drawDiscoveredField();
}

void 
pause() {}

void 
placeMines() {
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

void 
setMineCounts() {
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

int 
checkCell(int cell) {
	if (cell == 9) {
		return 1;
	}
	return 0;
}

void 
printField() {
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

void 
gameOver() {
	gameover = 1;
	for (int x=0; x<10; x++)
	{
		for (int y=0; y<10; y++)
		{
			discoveredField[x][y] = (field[x][y] == 0) ? 8 : field[x][y];
		}
	}
	drawDiscoveredField();

}
