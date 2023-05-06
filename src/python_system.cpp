
#ifdef HAVE_PYTHON
#include "model-sys/structures.hpp"
#include "python_system.hpp"
#include "directory.hpp"

#include <pybind11/embed.h>
namespace py = pybind11;

#include <iostream>
#include <string>

const std::string PythonSystem::m_stateID = "PYTHON_SYSTEM";

PythonSystem::PythonSystem(session_data_ptr session_data)
    : StateBase(session_data)
    , m_directory(new Directory())
{
}

PythonSystem::~PythonSystem()
{
    std::cout << "~PythonSystem()" << std::endl;
}


/*
 * Python Hook - Setup C++ Fucntion Hooks
 */
PYBIND11_EMBEDDED_MODULE(bbs_io, bbs_io)
{
    /*
    // Terminal
    bbs_io.def("pyGetTermHeight" , pyGetTermHeight);
    bbs_io.def("pyGetTermWidth"  , pyGetTermWidth);

    // Base I/o Input
    bbs_io.def("pyGetKey"        , pyGetKey);
    bbs_io.def("pyGetKeyExtended", pyGetKeyExtended);
    bbs_io.def("pyGetLine"       , pyGetLine);
    bbs_io.def("pyInputField"    , pyInputField);

    // Base I/o Output
    bbs_io.def("pyAnsiPrintf"    , pyAnsiPrintf);
    bbs_io.def("pyPutLine"       , pyPutLine);
    bbs_io.def("pyPipe2Ansi"     , pyPipe2Ansi);
    bbs_io.def("pyPipe2String"   , pyPipe2String);

    bbs_io.def("pyStartPause"    , pyStartPause);
    */
}


/**
 * @brief Handles Updates or Data Input from Client
 */
void PythonSystem::update(const std::string &character_buffer, const bool &is_utf8)
{
    if(!m_is_active)
    {
        return;
    }

    // Receiving Input, store where we can share it with the script.
    
    
}

/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 *        This is only called when switch to the state, not for menu instances.
 * @return
 */
bool PythonSystem::onEnter()
{
    // Start up Python Here,, probably earlier i think.. hmm
    // Script in Thread of External Program    
    
    m_is_active = true;
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool PythonSystem::onExit()
{
    // Cleanup Here
    m_is_active = false;
    return true;
}

/**
 * @brief Creates a Thread for Script Execution (Testing)
 * This will keep async data coming into the cass where it can be picked up. 
 * @param path
 * @return 
 */
std::thread PythonSystem::createThread(const std::string &path)
{    
    return std::thread([&] { 
        py::scoped_interpreter guard{}; // start the interpreter and keep it alive        
        py::object scope = py::module_::import("__main__").attr("__dict__");
        py::eval_file(path.c_str(), scope); 
    });
}
    
/*
 * Python Interface pass script and reference to user record.
 */
void PythonSystem::pyRunScript(const std::string &script)
{
    std::cout << "\x1b[0;1;16mpython test.. are we getting here!!" << std::endl;
    std::string path = GLOBAL_SCRIPT_PATH;
    path.append(script);
    try {
        
        //py::eval_file(path.c_str(), scope);
        m_script_thread = createThread(path);

    } catch(py::error_already_set const &) {
        
        std::cout << "PyBind11 Error - " << std::endl;
        PyErr_Print();        
    }

    return;
}

#endif // HAVE_PYTHON