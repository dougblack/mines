// mylib.h

#define OFFSET(r,c, rowlen)	((r)*(rowlen)+c)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern int field[10][10];
extern int discoveredField[10][10];

int sqran(int seed);
int qran();
int qran_range(int min, int max);
