// mylib.c

int __qran_seed = 10;
int field[20][20];
int discoveredField[20][20];
// Seed the generator
int sqran(int seed)
{
	int old = __qran_seed;
	__qran_seed = seed;
	return old;
}

// Returns a random integer
int qran()
{
	__qran_seed = 1664525 * __qran_seed+1013904223;
	return (__qran_seed >> 16) & 0x7FFF;
}

// Returns an integer in the given range
int qran_range(int min, int max)
{
	return (qran() * (max-min)>>15) + min;
}

