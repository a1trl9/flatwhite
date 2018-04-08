#include <Python.h>
#include "dr.h"
#include "ed.h"
#include "ngram.h"
#include "tf_idf.h"
#include "hashmap.h"

static PyObject* ParamError;

static PyObject* ged(PyObject* self, PyObject* args) {
    const Py_UNICODE* s;
    const Py_UNICODE* t; 
    int ss;
    int st;
    int match = 0;
    int mismatch = 1;
    int insert = 1;
    int delete = 1;
    int result = 0;

    if (!PyArg_ParseTuple(args, "u#u#|iiii", &s, &ss, &t, &st, &match, &mismatch, &insert, &delete)) {
        PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
        return NULL;
    }

    if ((match <= mismatch && (match >= insert || match >= delete)) || ((match <= insert || match <= delete) && match >= mismatch)) {
        PyErr_SetString(ParamError, "Match should be smallest or largest...");
        return NULL;
    }
    result = global_edit_distance(s, ss, t, st, match, mismatch, insert, delete);
    return Py_BuildValue("i", result);
}

static PyObject* led(PyObject* self, PyObject* args) {
    const Py_UNICODE* s;
    const Py_UNICODE* t; 
    int ss;
    int st;
    int match = 1;
    int mismatch = -1;
    int insert = -1;
    int delete = -1;
    int result = 0;

    if (!PyArg_ParseTuple(args, "u#u#|iiii", &s, &ss, &t, &st, &match, &mismatch, &insert, &delete)) {
        PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
        return NULL;
    }

    if ((match <= mismatch && (match >= insert || match >= delete)) || ((match <= insert || match <= delete) && match >= mismatch)) {
        PyErr_SetString(ParamError, "Match should be smallest or largest...");
        return NULL;
    }
    if (match == 0 || insert == 0 || delete == 0 || mismatch == 0) {
        PyErr_SetString(ParamError, "Pls do not use 0 as parameters since we use it as the pivot");
        return NULL;
    }
    result = local_edit_distance(s, ss, t, st, match, mismatch, insert, delete);
    return Py_BuildValue("i", result);
}

static PyObject* tf_idf_consine(PyObject* self, PyObject* args) {
    PyObject* list1;
    PyObject* list2;
    if (!PyArg_ParseTuple(args, "OO", &list1, &list2)) {
        PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
        return NULL;
    }
    size_t size1 = PyList_Size(list1);
    size_t size2 = PyList_Size(list2);
    if (size1 < 0 || size2 < 0) {
        PyErr_SetString(PyExc_TypeError, "Pls pass two lists");
        return NULL;
    }
    if (size1 != size2) {
        PyErr_SetString(ParamError, "Well... we cannot calc consine between vectors with different dimensions...");
        return NULL;
    }
    double result = calc_consine(list1, list2, size1);
    return Py_BuildValue("f", result);
}

// static PyObject* pick_best_by_ngram(PyObject* self, PyObject* args) {
//     PyObject* words;
//     PyObject* dictionary;
//     int index = 2;
//     if (!PyArg_ParseTuple(args, "OO|i", &words, &dictionary, &index)) {
//         PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
//         return NULL;
//     }
//     size_t size1 = PyList_Size(words);
//     size_t size2 = PyList_Size(dictionary);
//     if (size1 < 0 || size2 < 0) {
//         PyErr_SetString(PyExc_TypeError, "Pls pass two lists");
//         return NULL;
//     }
// }

static PyObject* ngram_distance(PyObject* self, PyObject* args) {
    const char* s1;
    const char* s2;
    int l1;
    int l2;
    int index = 2;
    if (!PyArg_ParseTuple(args, "s#s#|i", &s1, &l1, &s2, &l2, &index)) {
        PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
        return NULL;
    }
    int result = calc_ngram_distance(s1, l1, s2, l2, index);
    return Py_BuildValue("i", result);
}

static PyMethodDef methods[] = {
    {"ged", ged, METH_VARARGS, "ged(source, target, match, mismatch, insert, delete)\n"},
    {"led", led, METH_VARARGS, "led(source, target, match, mismatch, insert, delete)\n"},
    {"tf_idf_consine", tf_idf_consine, METH_VARARGS, "tf_idf_consine(vector1, vector2)\n"},
    {"ngram_distance", ngram_distance, METH_VARARGS, "ngram_distance(s1, s2)\n"},
    {NULL, NULL, 0, NULL}
};

#define INITERROR return NULL

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "flatwhite.cflatwhite",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC
PyInit_cflatwhite(void) {
    PyObject* module = PyModule_Create(&moduledef);
    if (module == NULL) {
        INITERROR;
    }
    ParamError = PyErr_NewException("param.error", NULL, NULL); 
    Py_INCREF(ParamError);
    PyModule_AddObject(module, "error", ParamError);
    return module;
}