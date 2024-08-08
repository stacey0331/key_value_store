#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "key_value_store.h"

namespace py = pybind11;

PYBIND11_MODULE(key_value_store_module, m) {
    py::register_exception<TypeMismatchError>(m, "TypeMismatchError");

    py::class_<KeyValueStore>(m, "KeyValueStore")
        .def(py::init<size_t, std::unique_ptr<EvictionPolicy>>())
        .def("set", &KeyValueStore::set)
        .def("get", &KeyValueStore::get)
        .def("delete", &KeyValueStore::del)
        .def("lpush", &KeyValueStore::lPush)
        .def("rpush", &KeyValueStore::rPush)
        .def("lpop", &KeyValueStore::lPop)
        .def("rpop", &KeyValueStore::rPop)
        .def("lrange", &KeyValueStore::lRange)
        .def("llen", &KeyValueStore::lLen)
        .def("sadd", &KeyValueStore::sAdd)
        .def("srem", &KeyValueStore::sRem)
        .def("smembers", &KeyValueStore::sMembers)
        .def("sismember", &KeyValueStore::sIsMember)
        .def("scard", &KeyValueStore::sCard);
}
