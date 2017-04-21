/*!
*\file      main.cpp
*\author    Wojciech Przybysz
*\date      9.04.2017
*/
#include <iostream>
//#include <boost/python.hpp>
//#include "server.hpp"
//#include <pyconfig.h>

//namespace py = boost::python;

int main(int argc, char* argv[]){

    std::cout << "In main: " << std::endl;
 /*   Py_Initialize();
    py::object_main_module = py::import("__main__");
    py::object_main_namespace = main_module.attr("__dict__");
    try{
        py::object noneType = py::ecet("result = 2*2",main_namespace);
        int res = py::extract<int>(main_namespace["result"]);
        std::cout << res << std::endl;
    }
    catch(py::error_already_set const &){
        PyErr_Print();
    }
    */
    return 0;
}