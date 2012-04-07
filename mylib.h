// mylib.h

#define OFFSET(r,c, rowlen)	((r)*(rowlen)+c)

typedef struct {
	int r;
	int c;
	int old_r;
	int old_c;
} indicator;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern int field[10][10];
extern int discoveredField[10][10];
extern int boardSize;

int sqran(int seed);
int qran();
int qran_range(int min, int max);
