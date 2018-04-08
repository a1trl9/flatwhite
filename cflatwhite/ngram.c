#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dr.h"
#include "hashmap.h"
#include "ngram.h"

#ifndef KEY_MAX_LENGTH
#define KEY_MAX_LENGTH (10)
#endif

typedef struct gram {
    char key[KEY_MAX_LENGTH]; 
    size_t count;
} gram;

void build_map(map_t result, const char* target, size_t length, unsigned index) {
    int error;
    char sub[index + 1];
    for (size_t i = 0; i <= length - index; i++) {
        gram* value;
        strncpy(sub, target + i, index);
        sub[index] = '\0';
        error = hashmap_get(result, sub, (void**)(&value));
        if (error == MAP_OK) {
            value->count += 1;
        } else {
            value = (gram*) malloc(sizeof(gram));
            memcpy(value->key, sub, index + 1);
            value->count = 1;
            hashmap_put(result, value->key, value);
        }
    }
}

void remove_map(map_t result, const char* target, size_t length, unsigned index) {
    int error;
    char sub[index + 1];
    for (size_t i = 0; i <= length - index; i++) {
        gram* value;
        strncpy(sub, target + i, index);
        sub[index] = '\0';
        error = hashmap_get(result, sub, (void**)(&value));
        if (error == MAP_OK) {
            hashmap_remove(result, sub);
            free(value);
        }
    }
    hashmap_free(result);
}

void build_map_collection(map_t* collections, const char** words, size_t length, unsigned index) {
    for (size_t i = 0; i < length; i++) {
        *(collections + i) = hashmap_new();
        build_map(*(collections + i), *(words + i), strlen(*(words + i)), index);
    }
}

// void get_closest_by_ngram(const char* s1, size_t l, const char** words, size_t length, int index) {
//     map_t collection[length];
//     build_map_collection(collection, words, length, index);
//     int min_distance = INT_MAX;
//     for (size_t i = 0; i < l - index; i++) {
//         map_t map = collection[i];
//         char sub[index + 1];
//         gram* value;
//         int error;
//         int common = 0;
//         char* ch = *(words + i);
//         size_t l2 = strlen(ch);
//         if (l2 > index && l > index) {
//             for (size_t j = 0; j <= l2 - index; j++) {
//                 memcpy(sub, ch + j, index);
//                 sub[index] = '\0';
//                 error = hashmap_get(map, sub, (void**)(&value));
//                 if (error == MAP_OK && value->count) {
//                     value->count -= 1;
//                     common += 1;
//                 }
//             }
//         }
//         if (*s1 == *ch) {
//             common += 1;
//         }
//         if (*(s1 + l - 1) == *(ch + l2 - 1)) {
//             common += 1;
//         }
//         int distance = 6 + l + l2 - 2 * index - 2 * common;
//     }
// }

int inner_calc_ngram_distance(map_t map, const char* s1, size_t l1, const char* s2, size_t l2, unsigned index) {
    int common = 0;
    if (l1 <  index || l2 < index) {
        int d1 = l1 + 1 - index;
        int d2 = l2 + 1 - index;
        return MAX(0, d1) + MAX(0, d2);
    }
    if (l1 >= index && l2 >= index) {
        char sub[index + 1];
        gram* value;
        int error;
        build_map(map, &s1[0], l1, index);
        for (size_t i = 0; i <= l2 - index; i++) {
            strncpy(sub, s2 + i, index);
            sub[index] = '\0';
            error = hashmap_get(map, sub, (void**)(&value));
            if (error == MAP_OK && value->count) {
                value->count -= 1;
                common += 1;
            }
        }
        remove_map(map, &s1[0], l1, index);
    }
    return 6 + l1 + l2 - 2 * index - 2 * common;
}

int calc_ngram_distance(const char* s1, size_t l1, const char* s2, size_t l2, unsigned index) {
    map_t map = hashmap_new();
    char ns1[l1 + 2];
    char ns2[l2 + 2];
    ns1[0] = '#';
    ns2[0] = '#';
    strncpy(&ns1[1], s1, l1);
    strncpy(&ns2[1], s2, l2);
    ns1[l1 + 1] = '#';
    ns2[l2 + 1] = '#';
    return inner_calc_ngram_distance(map, &ns1[0], l1 + 2, &ns2[0], l2 + 2, index);
}

// int main() {
//     char l1[5] = "abasd";
//     char l2[6] = "absdd";
//     calc_ngram_distance(&l1[0], 5, &l2[0], 6, 2);
//     return 0;
// }