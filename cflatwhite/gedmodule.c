#include <Python.h>
#include "ed.h"

static PyObject* ParamError;

static PyObject* ged(PyObject* self, PyObject* args) {
    const Py_UNICODE* s;
    const Py_UNICODE* t; 
    int ss;
    int st;
    int match = 0;
    int mismatch = 1;
    int gap = 1;
    int result = 0;

    if (!PyArg_ParseTuple(args, "u#u#|iii", &s, &ss, &t, &st, &match, &mismatch, &gap)) {
        PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
        return NULL;
    }

    if ((match <= mismatch && match >= gap) || (match <= gap && match >= mismatch)) {
        PyErr_SetString(ParamError, "Match should be smallest or largest...");
        return NULL;
    }
    result = global_edit_distance(s, ss, t, st, match, mismatch, gap);
    return Py_BuildValue("i", result);
}

static PyObject* led(PyObject* self, PyObject* args) {
    const Py_UNICODE* s;
    const Py_UNICODE* t; 
    int ss;
    int st;
    int match = 1;
    int mismatch = -1;
    int gap = -1;
    int result = 0;

    if (!PyArg_ParseTuple(args, "u#u#|iii", &s, &ss, &t, &st, &match, &mismatch, &gap)) {
        PyErr_SetString(PyExc_TypeError, "Invalid Arguments");
        return NULL;
    }

    if ((match <= mismatch && match >= gap) || (match <= gap && match >= mismatch)) {
        PyErr_SetString(ParamError, "Match should be smallest or largest...");
        return NULL;
    }
    if (match == 0 || gap == 0 || mismatch == 0) {
        PyErr_SetString(ParamError, "Pls do not use 0 as parameters since we use it as the pivot");
        return NULL;
    }
    result = local_edit_distance(s, ss, t, st, match, mismatch, gap);
    return Py_BuildValue("i", result);
}

static PyMethodDef methods[] = {
    {"ged", ged, METH_VARARGS, "ged(source, target, match, mismatch, gap)\n"},
    {"led", led, METH_VARARGS, "led(source, target, match, mismatch, gap)\n"},
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