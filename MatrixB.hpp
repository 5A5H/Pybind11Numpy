#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include <iomanip>

// including the pybind11 headder
#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class MatrixB
{
    public:
    MatrixB(size_t size)
    {
       DataStorage.reserve(size);
       for (size_t i=0; i<size; i++) 
       {
           double ii = i;
           DataStorage.push_back({ii+(ii+0)/10, ii+(ii+1)/10, ii+(ii+2)/10});
       }
       
    }
    double* data()
    {
        return DataStorage[0].data();
    }
    size_t size()
    {
        return DataStorage.size();
    }
    double sum()
    {
        double result = 0.0;
        for (auto v : DataStorage)
        for (auto d : v)
        result += d;
        return result;
    }
    
    private:
    std::vector<std::array<double,3>> DataStorage;
};

void init_MatrixB(py::module &m)
{
    py::class_<MatrixB, std::shared_ptr<MatrixB> >(m, "MatrixB")
    .def(py::init<size_t>())
    .def("sum", &MatrixB::sum)
    .def("size", &MatrixB::size)
    .def("data",[](MatrixB &b)
    {
        return py::array_t<double>(
            { b.size(), (size_t)3 },
            { sizeof(double) * 3, sizeof(double) },
            b.data(),
            py::capsule(b.data(), [](void *f){}));
    });
}