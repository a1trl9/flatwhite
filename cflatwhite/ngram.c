#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dr.h"
#include "hashmap.h"
#include "ngram.h"

#ifndef KEY_MAX_LENGTH
#define KEY_MAX_LENGTH (16)
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
        memcpy(sub, target + i, index);
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
        memcpy(sub, target + i, index);
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

int calc_ngram_distance(const char* s1, size_t l1, const char* s2, size_t l2, unsigned index) {
    int common = 0;
    if (l1 + 1 <  index || l2 + 1 < index) {
        int d1 = l1 + 1 - index;
        int d2 = l2 + 1 - index;
        return MAX(0, d1) + MAX(0, d2);
    }
    if (l1 >= index && l2 >= index) {
        map_t map = hashmap_new();
        char sub[index + 1];
        gram* value;
        int error;
        build_map(map, &s1[0], l1, index);
        for (size_t i = 0; i <= l2 - index; i++) {
            memcpy(sub, s2 + i, index);
            sub[index] = '\0';
            error = hashmap_get(map, sub, (void**)(&value));
            if (error == MAP_OK && value->count) {
                value->count -= 1;
                common += 1;
            }
        }
        remove_map(map, &s1[0], l1, index);
    }
    common += 2;
    for (size_t i = 0; i < index - 1; i++) {
        if (*(s1 + i) != *(s2 + i)) {
            common -= 1;
            break;
        }
    }
    for (size_t i = 0; i < index - 1; i++) {
        if (*(s1 + l1 - 1 - i) != *(s2 + l2 - 1 - i)) {
            common -= 1;
            break;
        }
    }
    return 6 + l1 + l2 - 2 * index - 2 * common;
}

// int main() {
//     map_t collection[5];
//     const char* cands[3] = {"abc", "aes", "aws"}; 
//     build_map_collection(collection, &cands[0], 3, 2);
//     return 0;
// }