# Pybind <-> Numpy interaction

In this project i investigate how my own C++ code can be linked with numpy. The code is made available to python via the *pybind11* library, which also supports the numpy compatibility.

## The Goal:
The idea is that c++ code is used to operate complex and ressorce consuming algorithms on large amounts of data. The python/numpy binding should be used to effectively read these large amounts of data and/or to make them available for benchmarking. 

Two initial thoughts where:
* Benchmarking of material subroutines: Typically the interface to material subroutines are build using referenced input variables as multiple return types are required. The prototype of such an application is given by a function, that takes an array like structure (e.g. numpy ndarray )as input and while not having an acutal return type, modifies its input to the result.

* Getting access to c++ data as a numpy array, without copying the data. This task is actually a little more complex as the data needs to be translated to an numpy array strcture.

## Some Remarks:
A restriction of python is that most standard data types are immutable, such that
```python
>>> a = 9
9
>>> a = 8
8
```
the first command does create a variable `a` and assignes the value `9` which is helt in some location in memory. The second command does not nessecarily overwrite the memory at the address of `a` where `9` is saved, but just reassignes a with a new memory association where the value `8` is saved.

## The Python-Buffer Protocol
Althoug this seems to be complicated at the first glance it actually is not so much. Also it reminds me to MPI-message parsing. The python buffer protocol is a way of describing python how to access memory which is not managed by python. That beeing said, it can be used to 'visalize' a numpy array to python, which actually acts on my c-data.
The implementation is provided in the code, but I like to introduce one thing upfornt to provide a little more explanation.

The python buffer protocol works such that on the c-side a buffer must be provided. This buffer is an array of pointers, pointing to the data in a contiguous manner.

> The buffer must provide a contiguous representation of the data ,while the data must not be contiguous. This way only the complexity of building the buffer (array of pointers to the data) increases if the actual data is not contiguous.

Futhermore the protocoll provides the instructions how to read this buffer as a numpy array. This means how many intries per row etc.

The buffer protocol is represented by a struct:
```c++
struct buffer_info {
    // pointer to the start of the buffer
    void *ptr;
    // size (in bytes) of a single entry 
    size_t itemsize;
    // a format desciptor (pybind provides some)
    std::string format;
    // number of dimensions of the numpy array
    int ndim;
    // vector containing the length of each dimension
    std::vector<size_t> shape;
    // vector containing the length of each dimension (in bytes)
    std::vector<size_t> strides;
};
```
**Example :**
Say we have the matrix **A**
```math
\left(
\begin{array}{ccc} 
1 & 2 & 3 \\ 
4 & 5 & 6 
\end{array}
\right)
```

saved in c within a class as a `std::vector<double>`. The `std::vector` represents a contiguous data array in memory.

The binding code is:
```c++
    .def_buffer(
        [](MatrixA &mat) -> py::buffer_info
        {
            auto MatrixBuffer = 
            py::buffer_info
            (
                // 1. a pointer to the buffer (start of the memoryblock with contiguous data)
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
```

## Ressources:
[Pythons C-API Docu on Buffer Protocol](https://docs.python.org/3/c-api/b???uffer.html)

>A pointer to the start of the logical structure described by the buffer fields. This can be any location within the underlying physical memory block of the exporter. For example, with negative strides the value may point to the end of the memory block.

For contiguous arrays, the value points to the beginning of the memory block.


