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

#include <FunctionA.hpp>
#include <MatrixA.hpp>
#include <MatrixB.hpp>

// list of Function Declarations
void init_Function(py::module &m);

// a standard c++ matrix class
class Matrix 
{
    public:
        Matrix(size_t rows, size_t cols) : m_rows(rows) , m_cols(cols)
        {
            m_data = new double[rows*cols];
            for (int i = 0; i< rows*cols; i++) m_data[i] = (double)i; 
        }
        double *data() {return m_data;}
        size_t rows() const {return m_rows;}
        size_t cols() const {return m_cols;}
        void print() const
        {
            std::cout << std::setw(6) << std::setprecision(3);
            for (size_t row = 0; row<m_rows; row++)
            {
                for (size_t col = 0; col<m_cols; col++)
                {
                    std::cout << m_data[row*m_cols+col] << "  ";
                }
                std::cout << std::endl;

            } 
        }
        double* vRef(int i, int j)
        {
            return &m_data[i*m_cols+j];
        }
    private:
        size_t m_rows, m_cols;
        double* m_data;
};

// generating pybind module
PYBIND11_MODULE(pymat, m) 
{
    m.doc() = "A Pybind11 Module";
    init_Function(m);
    init_ArrayFunction(m);
    init_MatrixA(m);
    init_MatrixB(m);
    auto p = py::class_<Matrix, std::shared_ptr<Matrix> >(m, "Matrix", py::buffer_protocol());
    p.doc() = "Matrix Class";
    p.def("__repr__",[](const Matrix &a){return "<Matrix>";});
    p.def(py::init<size_t, size_t>());
    p.def("print", &Matrix::print, py::call_guard<py::scoped_ostream_redirect>());
    p.def("vRef", &Matrix::vRef, "This does not work as python types such as floats are imutable!");
    p.def("ndarray",[](Matrix &a) -> py::array_t<double>
    {
        auto result = py::array_t<double>(a.rows()*a.cols());
        result.resize({a.rows(),a.cols()});
        py::buffer_info result_buf = result.request();
        double* NumpyPtr = (double*)result_buf.ptr;
        double* Ptr_2_ActualData = a.data();
        // std::cout << "Data i = 1, j = 0 : " << std::endl;
        // std::cout << "Ptr   : " << &Ptr_2_ActualData[1*a.cols()+0] << std::endl;
        // std::cout << "Value : " << (Ptr_2_ActualData[1*a.cols()+0]) << std::endl;
        for (int i=0; i<a.rows()*a.cols();i++) NumpyPtr[i] = Ptr_2_ActualData[i];
        return result;
    }
    , py::call_guard<py::scoped_ostream_redirect>()
    );
    p.def_buffer(
        [](Matrix &mat) -> py::buffer_info
        {
            auto MatrixBuffer = 
            py::buffer_info
            (
                // 1. a pointer to where the data starts
                mat.data(),
                // 2. the size of each individual type (homogenous data -> all types are equal)
                sizeof(double),
                // 3. python format (python must know the type, while standard types are supported)
                py::format_descriptor<double>::format(),
                // 4. dimensions of the resulting numpy array
                2,
                // 5. number of entires in each dimension
                { mat.rows(), mat.cols() },
                // 6. strides (in bytes) for each index (i.e. length of a row, col, etc. : array(0,0) -> internalarray[0*stride_dim1+0*stride_dim2], array(1,0) -> internalarray[1*stride_dim1+0*stride_dim2] )
                { sizeof(double)*mat.cols() ,sizeof(double)}
            );
            return MatrixBuffer;
        }
    );
}

// implementation
void init_Function(py::module &m)
{
    m.def("HelloWorld", []() 
    {
        std::cout << "HelloWorld!" << std::endl; 
    } 
    ,py::call_guard<py::scoped_ostream_redirect>()
    );
}