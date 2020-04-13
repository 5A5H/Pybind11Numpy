cmake_minimum_required(VERSION 3.0)
project(pymat_PROJECT VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
find_package(pybind11 REQUIRED)


pybind11_add_module(pymat pymat.cpp )
target_include_directories(pymat SYSTEM PRIVATE ${PROJECT_SOURCE_DIR} )


# Some usefull CMake Commands:
# target_link_libraries(Module PRIVATE LiraryName)
# add_executable(myEigenTest Eigen/myEigenTest.cpp)