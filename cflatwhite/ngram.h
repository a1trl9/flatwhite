#include <stddef.h>
#include "hashmap.h"

int calc_ngram_distance(const char*, size_t, const char*, size_t, unsigned);

void build_map_collection(map_t*, const char**, size_t, unsigned);