// main.c

#include "main.h"

enum {BLACKIDX, WHITEIDX, REDIDX, BLUEIDX, GREENIDX, YELLOWIDX, LIGHTGRAYIDX};
u16 colors[] = {BLACK, WHITE, RED, BLUE, GREEN, YELLOW, LIGHTGRAY};
indicator ind = {5, 5, 5, 5}; 
int playerloses = 0;
int cellsSwept = 0;
int correctFlags = 0;
int showTitle = 1;
int seed = 0;
int playerwins = 0;
int MINE_NUM = 4;
int boardSize = 4;

int main()
{

  /* SETUP COLOR PALETTE AND DISPLAY MODE */
  int numcolors = sizeof(colors) / sizeof(colors[0]);
  for (int i = 0; i < numcolors; i++)
  {
    PALETTE[i] = colors[i];
  }

  REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;

  while(1) 
  {

    /* SHOW TITLE SCREEN */
    titleScreen();

    /* INITIALIZE MINEFIELD */
    setupField();

    /* PROCESS INPUT; UPDATE GAME STATE */
    mainRunLoop();

    /* CHECK WIN AND LOSS CONDITIONS */
    checkVictory();

    /* RESET GAME STATE */
    showTitle = 1;
    playerwins = 0;
    playerloses = 0;

  }
}

void checkVictory() {
  if (playerwins)
    win();
  else if(playerloses)
    gameOver();
}

void mainRunLoop()
{
  while(!playerloses && !playerwins) {
    keyHandle();
    if (key_hit(KEY_SELECT)) {
      showTitle = 1;
      break;
    }
    waitForVBlank();
  }
}
void setupField()
{
  ind.r = boardSize / 2;
  ind.c = boardSize / 2;
  for (int x = 0; x < boardSize; x++) {
    for (int y = 0; y < boardSize; y++) {
      discoveredField[x][y] = 0;
      field[x][y] = 0;
    }
  }
  sqran(seed);
  fillScreen4(0);
  flipPage();
  fillScreen4(0);

  placeMines();
  setMineCounts();
  drawDiscoveredField();
  drawFieldBorders();
  flipPage();
  drawDiscoveredField();
  drawFieldBorders();
  drawIndicator(ind);
  flipPage();
}

/* SHOW TITLESCREEN UNTIL INPUT */
void titleScreen() 
{
  drawImage4(0,0,240,160,title);
  flipPage();
  while (showTitle) {
    key_poll();
    if (key_hit(KEY_START)) {
      showTitle=0;
    }
    if (key_hit(KEY_UP)) {
      MINE_NUM+=5;
      if(boardSize < 10) boardSize+=3;
    }
    seed++;
  }
}

/* PROCESS INPUT */
void keyHandle() 
{
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
  if (keyHandled && !playerloses) {
    clearIndicator(ind);
    drawIndicator(ind);	
    flipPage();
    clearIndicator(ind);
    drawDiscoveredField();
  }
}

/* SWEEP THE GIVEN CELL IN THE MINEFIELD. RECURSIVELY SWEEPS
   NEIGHBORING CELLS UNTIL ALL NONEMPTY CELLS HAVE BEEN DISCOVERED. */
void sweep(int r, int c) 
{ 
  if (r >= 0 && r < boardSize && c >= 0 && c < boardSize) { // Only sweep if within field bounds
    /* Check for mine */
    if (field[r][c] == 9) {
      playerloses=1;
    }

    if ((discoveredField[r][c] == 0) || (discoveredField[r][c] == 7)) {

      if (discoveredField[r][c] == 7)
        correctFlags--;

      int cell = field[r][c];

      if (cell == 0) {
        cell = 8;
        discoveredField[r][c] = 8;

        /* Sweep all neighboring cells */
        sweep(r,c-1);
        sweep(r-1,c-1);
        sweep(r+1,c-1);
        sweep(r,c+1);
        sweep(r-1,c+1);
        sweep(r+1,c+1);
        sweep(r-1,c);
        sweep(r+1,c);
      }
      cellsSwept++;
      discoveredField[r][c] = cell;

      /* Check if all cells have been swept */
      if (cellsSwept == (boardSize * boardSize - MINE_NUM)) {
        playerwins = 1;
      }
    }
  }
}

/* DISPLAY WIN SCREEN */
void win()
{
  drawImage4(0,0,240,160,winscreen);
  flipPage();
  int showWinScreen = 1;
  while (showWinScreen) {
    key_poll();
    if (key_hit(KEY_START)) {
      showWinScreen = 0;
    }
  }

}

void waitForVBlank()
{
  while(SCANLINECOUNTER > 160);
  while(SCANLINECOUNTER < 160);
}

/* CHANGE INDICATOR LOCATION */
void indicatorMove(int delta_x, int delta_y) 
{

  int newR = ind.r + delta_y;
  int newC = ind.c + delta_x;
  if ((newR >= 0) && (newR < boardSize) && (newC >= 0) && (newC < boardSize)) {
    ind.old_r = ind.r;
    ind.old_c = ind.c;
    ind.r = newR;
    ind.c = newC;
  }
}

/* Flip to opposing page */
void flipPage() 
{
  if (REG_DISPCTL & BUFFER1FLAG) {
    REG_DISPCTL &= ~BUFFER1FLAG;
    videoBuffer = BUFFER1;
  }
  else {
    REG_DISPCTL |= BUFFER1FLAG;
    videoBuffer = BUFFER0;
  }
}

void placeFlag() 
{
  discoveredField[ind.r][ind.c] = 7;	
  if (field[ind.r][ind.c]==9)
    correctFlags++;
  if (correctFlags == MINE_NUM)
    playerwins = 1;
  drawDiscoveredField();
}

void pause() {}

/* Randomly place mines */
void placeMines() 
{
  int minesPlaced = 0;
  while (minesPlaced < MINE_NUM) {
    int x = qran_range(0,boardSize);
    int y = qran_range(0,boardSize);
    if (field[x][y] != 9) {
      field[x][y] = 9;
      minesPlaced++;
    }	
  }
  printField();
  DEBUG_PRINT("\n=======================================\n");
}

/* CALCULATE THE MINE PROXIMITY OF NON-MINE CELLS */
void setMineCounts() 
{
  for (int x=0; x<boardSize; x++)
  {
    for (int y=0; y<boardSize; y++)
    {
      int checkLeft = 1;
      int checkRight = 1;
      int checkTop = 1;
      int checkBottom = 1;
      if (field[x][y] != 9) 
      {	
        int cell = 0;
        if (y==0)	checkLeft = 0;
        else if (y==boardSize-1)	checkRight = 0;
        if (x==0)	checkTop = 0;
        else if (x==boardSize-1)	checkBottom = 0;

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

int checkCell(int cell) 
{
  if (cell == 9) {
    return 1;
  }
  return 0;
}

void printField() 
{
  for (int x=0; x<boardSize; x++)
  {
    for (int y=0; y<boardSize; y++)
    {
      if (field[x][y] == 9)
        DEBUG_PRINT("X, ");
      else
        DEBUG_PRINTF("%d, ", field[x][y]);
    }
    DEBUG_PRINT("\n");
  }
}

void gameOver()
{
  drawExplosion(ind.r, ind.c);
  for (int x=0; x<boardSize; x++)
  {
    for (int y=0; y<boardSize; y++)
    {
      discoveredField[x][y] = (field[x][y] == 0) ? 8 : field[x][y];
    }
  }
  drawDiscoveredField();
  flipPage();

  int showGameOverScreen = 0;
  while (!showGameOverScreen)
  {
    key_poll();
    if (key_hit(KEY_START))
      showGameOverScreen=1;
  } 
  drawImage4(0,0,240,160,gameoverscreen);
  flipPage();
  while (showGameOverScreen)
  {
    key_poll();
    if (key_hit(KEY_START))
      showGameOverScreen=0;
  } 
}

/* DRAW CUSTOM EXPLOSION ANIMATION */
void drawExplosion(int r, int c)
{
  int x = 10+r*14+2;
  int y = 50+c*14+3;
  drawImage4(x,y,10,10,mineone);
  flipPage();
  waitFourishSeconds();
  drawImage4(x,y,10,10,minetwo);
  flipPage();
  waitFourishSeconds();
  drawImage4(x,y,10,10,minethree);
  flipPage();
  waitFourishSeconds();
  drawImage4(x,y,10,10,minefour);
  flipPage();
  waitFourishSeconds();
  drawImage4(x,y,10,10,minefive);
  flipPage();

}

/* This is definitely not four seconds but whatever. */
void waitFourishSeconds()
{
  for (int i=0; i<240;i++)
    waitForVBlank();
}
