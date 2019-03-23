#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include "glm/glm.hpp"

#define v_xyz(obj) &obj.x, &obj.y, &obj.z

PyObject * meth_raycast(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"source", "direction", "mesh", "stride", NULL};

    glm::vec3 source;
    glm::vec3 direction;
    Py_buffer view;
    int stride = sizeof(glm::vec3);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)y*|i", keywords, v_xyz(source), v_xyz(direction), &view, stride)) {
        return 0;
    }

    Py_RETURN_NONE;
}

PyMethodDef module_methods[] = {
    {"raycast", (PyCFunction)meth_raycast, METH_VARARGS | METH_KEYWORDS, 0},
    {0},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "mesh_raycast", 0, -1, module_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mesh_raycast() {
    PyObject * module = PyModule_Create(&module_def);
    return module;
}
