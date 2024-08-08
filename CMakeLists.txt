cmake_minimum_required(VERSION 3.19)

project(key_value_store VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_OSX_ARCHITECTURES "arm64")

option(BUILD_PYBIND "Build the pybind11 module" ON)
option(BUILD_TESTS "Build the Google Test tests" ON)

add_library(key_value_store_lib
    src/key_value_store.cpp
)

target_include_directories(key_value_store_lib PRIVATE include)

if(BUILD_PYBIND)
    find_package(pybind11 REQUIRED)
    pybind11_add_module(key_value_store_module src/key_value_store_module.cpp)
    target_include_directories(key_value_store_module PRIVATE include)
    target_link_libraries(key_value_store_module PRIVATE key_value_store_lib)
endif()

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/3e3b44c300b21eb996a2957782421bc0f157af18.zip
    )
    FetchContent_MakeAvailable(googletest)
    
    enable_testing()

    add_executable(
        key_value_store_test
        tests/key_value_store_test.cpp
    )

    target_include_directories(key_value_store_test PRIVATE include)
    target_link_libraries(key_value_store_test PRIVATE key_value_store_lib GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(key_value_store_test)
endif()
