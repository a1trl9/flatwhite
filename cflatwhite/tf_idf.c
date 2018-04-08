#include <math.h>
#include <stdlib.h>
#include <Python.h>
#include "tf_idf.h"

double calc_idf(int doc_freq, int doc_number) {
    if (doc_freq == 0) {
        return 0;
    }
    return log(1 + (float) doc_number / (float) doc_freq);
}

float calc_tf(int term_freq) {
    if (term_freq == 0) {
        return 0;
    }
    return 1 + log(term_freq);
}

double calc_consine(PyObject* o1, PyObject* o2, size_t size) {
    double result = 0;
    double d1 = 0;
    double d2 = 0;
    for (size_t i = 0; i < size; i++) {
        PyObject* e1 = PyList_GetItem(o1, i);
        PyObject* e2 = PyList_GetItem(o2, i);
        double v1 = PyFloat_AsDouble(e1);
        double v2 = PyFloat_AsDouble(e2);
        result +=  v1 * v2;
        d1 += v1 * v1;
        d2 += v2 * v2;
    }
        d1 = sqrtf(d1);
        d2 = sqrtf(d2);
    return result / (d1 * d2);
}