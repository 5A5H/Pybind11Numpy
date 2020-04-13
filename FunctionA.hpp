// The Module pymat

// include c++ headder
#include <memory>
#include <iostream>
#include <iomanip>

// including the pybind11 headder
#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

py::array_t<double> arrayF(py::array_t<double> input1) 
{

    // Get info from input1
    py::buffer_info buf1 = input1.request();

    if (buf1.ndim !=1)
    {
        throw std::runtime_error("Number of dimensions must be one");
    }


    //Apply resources
    auto result = py::array_t<double>(buf1.size);
    py::buffer_info buf3 = result.request();

    //Obtain numpy.ndarray data pointer
    auto ptr1 = static_cast<double *>(buf1.ptr);
    double* ptr3 = (double*)buf3.ptr;

    //Pointer visits numpy.ndarray
    for (int i = 0; i < buf1.shape[0]; i++)
    {
        ptr3[i] = ptr1[i] + 1.0;
        ptr1[i] -= 1.0; // here is how to modify the input
    }

    return result;
 
}

void init_ArrayFunction(py::module &m)
{
    m.def("ArrayF"
    ,&arrayF
    ,py::call_guard<py::scoped_ostream_redirect>()
    );
}