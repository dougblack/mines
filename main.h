// main.h

#include "input.h"
#include <debugging.h>

extern int field[10][10];
extern int boardSize;
extern int discoveredField[10][10];

void waitForVBlank();
void indicatorMove(int delta_x, int delta_y);
void flipPage();
void keyHandle();
void sweep(int r, int c);
void placeFlag();
void pause();
void placeMines();
void printField();
void setMineCounts();
int checkCell(int cell);
void gameOver();
void win();
void drawExplosion(int r, int c);
void waitFourishSeconds();
