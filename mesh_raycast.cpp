#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include "glm/glm.hpp"

#define v_xyz(obj) &obj.x, &obj.y, &obj.z

inline glm::vec3 & vertex(void * ptr, int stride, int idx) {
    return *(glm::vec3 *)(((char *)ptr) + stride * idx);
}

PyObject * meth_raycast(PyObject * self, PyObject * args, PyObject * kwargs) {
    static char * keywords[] = {"source", "direction", "mesh", "stride", NULL};

    glm::vec3 source;
    glm::vec3 direction;
    Py_buffer mesh;
    int stride = sizeof(glm::vec3);

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(fff)(fff)y*|i", keywords, v_xyz(source), v_xyz(direction), &mesh, stride)) {
        return 0;
    }

    if (mesh.len % (stride * 3)) {
        PyErr_Format(PyExc_ValueError, "stride error");
        return 0;
    }

    glm::vec3 d = -glm::normalize(direction);
    PyObject * res = PyList_New(0);

    int num_triangles = (int)(mesh.len / (stride * 3));

    for (int i = 0; i < num_triangles; ++i) {
        const glm::vec3 a = vertex(mesh.buf, stride, i * 3);
        const glm::vec3 b = vertex(mesh.buf, stride, i * 3 + 1) - a;
        const glm::vec3 c = vertex(mesh.buf, stride, i * 3 + 2) - a;
        const glm::vec3 g = source - a;

        const float det = glm::determinant(glm::mat3(b, c, d));
        if (!det) {
            continue;
        }

        const float n = glm::determinant(glm::mat3(g, c, d)) / det;
        const float m = glm::determinant(glm::mat3(b, g, d)) / det;
        const float k = glm::determinant(glm::mat3(b, c, g)) / det;

        if (n >= 0.0f && m >= 0.0f && n + m <= 1.0f && k >= 0.0f) {
            const glm::vec3 pt = source + direction * k;
            const glm::vec3 norm = glm::normalize(glm::cross(b, c));
            const float dot = glm::dot(d, norm);

            PyObject * match = Py_BuildValue(
                "{sis(fff)s(fff)s(ff)sfsf}",
                "face", i,
                "point", pt.x, pt.y, pt.z,
                "normal", norm.x, norm.y, norm.z,
                "coeff", n, m,
                "distance", k,
                "dot", dot
            );
            PyList_Append(res, match);
            Py_DECREF(match);
        }
    }

    PyBuffer_Release(&mesh);
    return res;
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
