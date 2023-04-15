cmake_minimum_required(VERSION 3.4...3.18)
project(cubao_pybind)

add_subdirectory(pybind11)
pybind11_add_module(_cubao_pybind src/main.cpp)

target_compile_definitions(_cubao_pybind
                           PRIVATE VERSION_INFO=${CUBAO_PYBIND_VERSION_INFO})
