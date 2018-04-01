#include "ed.h"
#include <stdlib.h>
#include <Python.h>

int local_edit_distance(const Py_UNICODE* s, size_t ss, const Py_UNICODE* t, size_t st,
    int match, int mismatch, int gap
) {
    size_t rows = st + 1;
    size_t cols = ss + 1;
    int d1, d2, d3;
    unsigned short is_distance = match < mismatch ? 1 : 0;

    int *dp = malloc(rows * cols * sizeof(int));

    for (size_t i = 0; i < rows; i++) {
        dp[i * cols] = i * gap;
    }

    for (size_t i = 0; i < cols; i++) {
        dp[i] = i * gap;
    }

    for (size_t j = 1; j < rows; j++) {
        for (size_t k = 1; k < cols; k++) {
            d3 = s[k - 1] == t[j - 1] ? dp[(j - 1) * cols + k - 1] + match :
                dp[(j - 1) * cols + k - 1] + mismatch;
            d1 = dp[j * cols + k - 1] + gap;
            d2 = dp[(j - 1) * cols + k] + gap;
            if (is_distance) {
                dp[j * cols + k] = MIN(0, MIN(d3, MIN(d1, d2)));
            } else {
                dp[j * cols + k] = MAX(0, MAX(d3, MAX(d1, d2)));
            }
        }
    }
    int result = 0;
    for (size_t i = 0; i < rows * cols; i++) {
        if ((dp[i] < result && is_distance) || (dp[i] > result && !is_distance)) {
            result = dp[i];
        }
    }
    free(dp);
    return result;
}
