#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include "glm/glm.hpp"

#define v_xyz(obj) &obj.x, &obj.y, &obj.z

PyObject * meth_raycast(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"source", "direction", "mesh", "stride", NULL};

    glm::vec3 source;
    glm::vec3 direction;
    Py_buffer mesh;
    int stride = sizeof(glm::vec3);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)y*|i", keywords, v_xyz(source), v_xyz(direction), &mesh, stride)) {
        return 0;
    }

    PyBuffer_Release(&mesh);
    Py_RETURN_NONE;
}

PyObject * meth_iraycast(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"source", "direction", "mesh", "index", "stride", NULL};

    glm::vec3 source;
    glm::vec3 direction;
    Py_buffer mesh;
    Py_buffer index;
    int stride = sizeof(glm::vec3);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)y*y*|i", keywords, v_xyz(source), v_xyz(direction), &mesh, &index, &stride)) {
        return 0;
    }

    PyBuffer_Release(&index);
    PyBuffer_Release(&mesh);
    Py_RETURN_NONE;
}

PyObject * meth_reflect(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"vector", "normal", NULL};

    glm::vec3 vector;
    glm::vec3 normal;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)", keywords, v_xyz(vector), v_xyz(normal))) {
        return 0;
    }

    glm::vec3 result = glm::reflect(vector, normal);
    return Py_BuildValue("fff", result.x, result.y, result.z);
}

PyObject * meth_refract(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"vector", "normal", "eta", NULL};

    glm::vec3 vector;
    glm::vec3 normal;
    float eta;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)f", keywords, v_xyz(vector), v_xyz(normal), &eta)) {
        return 0;
    }

    glm::vec3 result = glm::refract(vector, normal, eta);
    return Py_BuildValue("fff", result.x, result.y, result.z);
}

PyObject * meth_normalize(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"vector", NULL};

    glm::vec3 vector;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)", keywords, v_xyz(vector))) {
        return 0;
    }

    glm::vec3 result = glm::normalize(vector);
    return Py_BuildValue("fff", result.x, result.y, result.z);
}

PyObject * meth_direction(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"source", "target", NULL};

    glm::vec3 source;
    glm::vec3 target;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)", keywords, v_xyz(source), v_xyz(target))) {
        return 0;
    }

    glm::vec3 result = glm::normalize(target - source);
    return Py_BuildValue("fff", result.x, result.y, result.z);
}

PyMethodDef module_methods[] = {
    {"raycast", (PyCFunction)meth_raycast, METH_VARARGS | METH_KEYWORDS, 0},
    {"iraycast", (PyCFunction)meth_iraycast, METH_VARARGS | METH_KEYWORDS, 0},
    {"reflect", (PyCFunction)meth_reflect, METH_VARARGS | METH_KEYWORDS, 0},
    {"refract", (PyCFunction)meth_refract, METH_VARARGS | METH_KEYWORDS, 0},
    {"normalize", (PyCFunction)meth_normalize, METH_VARARGS | METH_KEYWORDS, 0},
    {"direction", (PyCFunction)meth_direction, METH_VARARGS | METH_KEYWORDS, 0},
    {0},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "mesh_raycast", 0, -1, module_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mesh_raycast() {
    PyObject * module = PyModule_Create(&module_def);
    return module;
}
