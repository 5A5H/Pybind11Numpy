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

Preliminary:
To be represented as a numpy array, the data must lie in continuous memory, i.e. a buffer.
Lets suppose the target is to expose the data of matrix A:
```math
A = 
\begin{array}[ccc] 
1 & 2 & 3 \\ 
4 & 5 & 6 
\end{\array}
```
