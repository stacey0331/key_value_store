#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "key_value_store.h"

namespace py = pybind11;

PYBIND11_MODULE(key_value_store_module, m) {
    py::class_<KeyValueStore>(m, "KeyValueStore")
        .def(py::init<>())
        .def("lpush", &KeyValueStore::lPush)
        .def("lrange", &KeyValueStore::lRange);
}
