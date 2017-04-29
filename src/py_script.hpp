#ifndef PY_SCRIPT_HPP
#define PY_SCRIPT_HPP

#include <string>


/**
 * @class PyScript
 * @author Michael Griffin
 * @date 26/04/2017
 * @file py_script.hpp
 * @brief Python Interpeter
 */
class PyScript
{
public:
    PyScript();
    ~PyScript();
    
    
    void pathAppend(std::string &path);
    void pyInterpreter(std::string script);

};

#endif // PY_SCRIPT_HPP
