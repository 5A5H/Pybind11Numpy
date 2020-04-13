#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>

// including the pybind11 headder
#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class MatrixA
{
    public:
    MatrixA()
    {
       DataStorage = {1.0 ,2.0 ,3.0 ,4.0, 5.0, 6.0};
    }
    double* data()
    {
        return &(DataStorage[0]);
    }
    
    private:
    std::vector<double> DataStorage;
};

void init_MatrixA(py::module &m)
{
    py::class_<MatrixA, std::shared_ptr<MatrixA> >(m, "MatrixA", py::buffer_protocol())
    .def(py::init<>())
    .def_buffer(
        [](MatrixA &mat) -> py::buffer_info
        {
            auto MatrixBuffer = 
            py::buffer_info
            (
                // 1. a pointer to the buffer (buffer = array of pointer to data)
                mat.data(),
                // 2. the size of each individual type (homogenous data -> all types are equal)
                sizeof(double),
                // 3. python format (python must know the type, while standard types are supported)
                py::format_descriptor<double>::format(),
                // 4. dimensions of the resulting numpy array
                2,
                // 5. number of entires in each dimension
                { 2, 3 },
                // 6. strides (in bytes) for each index (i.e. length of a row, col, etc. : array(0,0) -> internalarray[0*stride_dim1+0*stride_dim2], array(1,0) -> internalarray[1*stride_dim1+0*stride_dim2] )
                { sizeof(double)*3 ,sizeof(double)}
            );
            return MatrixBuffer;
        }
    );
}