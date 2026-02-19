#define A 1
#define B 0

#if defined(A) && (!defined(B) || A + 3 > 2)
int ok = 1;
#else
int fail = 1;
#endif

