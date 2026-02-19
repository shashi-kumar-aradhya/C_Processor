#define A 0
#define B 1

#if A
int x = 1;
#elif B
int x = 2;
#elif 1
int x = 3;
#else
int x = 4;
#endif

