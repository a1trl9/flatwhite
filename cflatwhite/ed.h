#include <stdlib.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

int global_edit_distance(const int*, size_t, const int*, size_t, int, int, int);
int local_edit_distance(const int*, size_t, const int*, size_t, int, int, int);