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
int MINE_NUM = 10;
int boardSize = 10;

    int
main()
{
    int numcolors = sizeof(colors) / sizeof(colors[0]);
    for (int i = 0; i < numcolors; i++)
    {
        PALETTE[i] = colors[i];
    }

    REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;

    //drawImage4(0,0,240,160,title);
    drawImage4(0,0,240,160,title);
    flipPage();
    while (showTitle){
        key_poll();
        if (key_hit(KEY_START)) {
            showTitle=0;
        }
        if (key_hit(KEY_UP)) {
            MINE_NUM+=5;
        }
        seed++;
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
    while(!playerloses && !playerwins)
    {
        keyHandle();
        waitForVBlank();
    }
    
    if (playerwins)
        win();
    else
        gameOver();

    
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
    if (keyHandled && !playerloses) {
        clearIndicator(ind);
        drawIndicator(ind);	
        flipPage();
        clearIndicator(ind);
        drawDiscoveredField();
    }
}

void sweep(int r, int c) 
{ 
    if(r>=0 && r <10 && c>=0 && c<10) {
        if (field[r][c] == 9)
        {
            playerloses=1;
        }
        if ((discoveredField[r][c] == 0) || (discoveredField[r][c] == 7)) {
            if (discoveredField[r][c] == 7)
                correctFlags--;
            int cell = field[r][c];
            if (cell == 0) {
                cell = 8;
                discoveredField[r][c] = 8;

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
            if (cellsSwept==90)
            {
                playerwins = 1;
            }
        }
    }
}

    void 
win()
{
    drawImage4(0,0,240,160,winscreen);
    flipPage();
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
    if (field[ind.r][ind.c]==9)
        correctFlags++;
    if (correctFlags == MINE_NUM)
        playerwins = 1;
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
    drawExplosion(ind.r, ind.c);
    for (int x=0; x<10; x++)
    {
        for (int y=0; y<10; y++)
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
}

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

void waitFourishSeconds()
{
    for (int i=0; i<240;i++)
        waitForVBlank();
}
