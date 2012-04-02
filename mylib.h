// mylib.h

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern int field[20][20];

int sqran(int seed);
int qran();
int qran_range(int min, int max);
