// main.h

#include "input.h"

extern int field[20][20];

void waitForVBlank();
void indicatorMove(int delta_x, int delta_y);
void flipPage();
void keyHandle();
void sweep();
void placeFlag();
void pause();
