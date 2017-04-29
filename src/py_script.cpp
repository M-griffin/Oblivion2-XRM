#include "py_script.hpp"

#include "model-sys/structures.hpp"


#include <cmath>
#include <iostream>
#include <string>
//# include <csignal>


// Start of new Boost::Python Embedding.
#include <boost/python.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_DYN_LINK

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::python;
using namespace boost::filesystem;


/**
 * @class AcquireGIL
 * @author Michael Griffin
 * @date 26/04/2017
 * @file py_script.cpp
 * @brief Handle Python Thread State.
 */
class AcquireGIL final
{
public:
    AcquireGIL();
    ~AcquireGIL();


private:
    PyGILState_STATE gilState_;
   
};

AcquireGIL::AcquireGIL()
    : gilState_(PyGILState_Ensure()) 
{
    // nothing to do...
}

AcquireGIL::~AcquireGIL() 
{
    PyGILState_Release(gilState_);
}

PyScript::PyScript()
{
}

PyScript::~PyScript()
{
}



/*
 * Python Hook - get Terminal Height
 *
int pyGetTermHeight()
{
	return TERM_HEIGHT;
}

*
 * Python Hook - get Terminal Width
 *
int pyGetTermWidth()
{
	return TERM_WIDTH;
}

*
 * Python Hook - Display Ansi File
 *
void pyAnsiPrintf(std::string str)
{
    SESSION s(pyUser);
    if ((signed)str.size() > 0)
        s.ansiPrintf((char *)str.c_str());
    return;
}

// Hold ESC Sequence
static std::string KeyCombination;

*
 * Python Hook - Return ESC Sequence
 *
std::string pyGetKeyExtended()
{
    return KeyCombination;
}

*
 * Python Hook - Return Parsed String of MCI And PIPE Colors
 *
std::string pyPipe2String(std::string str)
{
	SESSION s(pyUser);
	return s.pipe2string(str);
}

*
 * Python Hook - Get Single Key Input
 *
std::string pyGetKey()
{
    SESSION s(pyUser);
    int ch = 0;

    ch = s.getkey(true);
    if (ch == 27)
    {
        KeyCombination.erase();
        KeyCombination.append(s.EscapeKey);
    }
    else
        KeyCombination.erase();

    return boost::lexical_cast<std::string>(ch);
}

*
 * Python Hook - Get Line Input
 *
std::string pyGetLine(int len)
{
    SESSION s(pyUser);

    char str[1024]={0};
    s.getline(str,len);
    std::string sstr = str;

    return sstr;
}

*
 * Python Hook - setup Input Field with Length
 *
std::string pyInputField(std::string str, int len)
{
	SESSION s(pyUser);

	char instr[1024]={0};

	// Make sure string in not passed to big!
	if ((signed)str.size() > 1024)
		str.erase(1023,(signed)str.size() - 1023);
	
	strcpy(instr, (char *)str.c_str());
		
    s.inputfield(instr, len);
    std::string sstr = instr;

    return sstr;
}

*
 * Python Hook - Write Out and Parse text for MCI and PIPE Codes
 *
void pyPipe2Ansi(std::string str)
{
    SESSION s(pyUser);
  //  s.errlog2((char *)str.c_str());
    if ((signed)str.size() > 0)
        s.pipe2ansi((char *)str.c_str());
    return;
}

*
 * Python Hook - Write Out Raw String
 *
void pyPutLine(std::string str)
{
    SESSION s(pyUser);
    if ((signed)str.size() > 0)
        s.putline((char *)str.c_str());
    return;
}

*
 * Python Hook - Animated Pause
 *
void pyStartPause()
{
    SESSION s(pyUser);
    s.startpause();
    return;
}


*
 * Python Hook - Setup C++ Fucntion Hooks
 *
BOOST_PYTHON_MODULE(bbs_io)
{

	// Access to Menu System
//	boost::python::class_<pyMenuSystem>("pyMenuSystem")
//		.def("set", &pyMenuSystem::set)
//		.def("readin", &pyMenuSystem::readin)
//		.def("docmd", &pyMenuSystem::docmd)
//		.def("clearcmd", &pyMenuSystem::clearcmd)
//		.def("startmenu", &pyMenuSystem::startmenu);


    // Terminal
    def("pyGetTermHeight" , pyGetTermHeight);
    def("pyGetTermWidth"  , pyGetTermWidth);

    // Base I/o Input
    def("pyGetKey"        , pyGetKey);
    def("pyGetKeyExtended", pyGetKeyExtended);
    def("pyGetLine"       , pyGetLine);
    def("pyInputField"    , pyInputField);

    // Base I/o Output
    def("pyAnsiPrintf"    , pyAnsiPrintf);
    def("pyPutLine"       , pyPutLine);
    def("pyPipe2Ansi"     , pyPipe2Ansi);
    def("pyPipe2String"   , pyPipe2String);

    def("pyStartPause"    , pyStartPause);
}
*/


void PyScript::pathAppend(std::string &path) 
{
#ifdef _WIN32
    path.append("\\");
#else
    path.append("/");
#endif
}


void PyScript::pyInterpreter(std::string script)
{
    // Setup scripts path.
    std::string path = GLOBAL_SCRIPT_PATH;
    pathAppend(path);
    path += script;

    /*
	sigset_t signal_set; // We don't need oldset in this program. You can add it,
                         //but it's best to use different sigsets for the second
                         //and third argument of sigprocmask. 
	sigemptyset(&signal_set);

	// Block Signals in Python, casue then the BBS doesn't get them.
	sigaddset(&signal_set, SIGHUP);
	sigaddset(&signal_set, SIGTERM);
	sigaddset(&signal_set, SIGINT);
	sigaddset(&signal_set, SIGILL);
	sigaddset(&signal_set, SIGABRT);
	sigaddset(&signal_set, SIGCHLD);
	sigaddset(&signal_set, SIGQUIT);
	sigaddset(&signal_set, SIGKILL);
	sigaddset(&signal_set, SIGWINCH);

	sigprocmask(SIG_BLOCK, &signal_set, NULL);
    */
    
    
    try
    {

        //function should be called before Py_Initialize() to inform
        //the interpreter about paths to Python run-time libraries.
        //Next, the Python interpreter is initialized with Py_Initialize(),
        //followed by the execution of a hard-coded Python script that
        //prints the date and time.

        //Py_SetProgramName(BBSPATH);

        /* Simple Test Code
        PyRun_SimpleString("result = 5 ** 2");

        object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
        object dictionary = module.attr("__dict__");
        object result = dictionary["result"];

        int result_value = extract<int>(result);
        std::cout << result_value << std::endl;

        dictionary["result"] = 20;
        PyRun_SimpleString("print result");

        initbbs_io(); // initialize Pointless

        PyRun_SimpleString("import bbs_io");
        PyRun_SimpleString("print bbs_io.add_five(4)");
        */

		// Testing
        //path += "MyModule.py";

		if (boost::filesystem::exists(path.c_str())) // does actually exist?
 		{
		    //if (boost::filesystem::is_regular_file(path.c_str()))        // is p a regular file?   
	        //	std::cout << path.c_str() << " size is " << boost::filesystem::file_size(path.c_str()) << endl;			 
		}
		else
		{
			std::cout << "Script not found: " << path.c_str() << std::endl;
			return;
		}

        std::cout << "Starting Boost_Python" << std::endl;
        
        //PyImport_AppendInittab((char*)"myextension", INIT_MODULE);
         //initbbs_io(); // initialize Pointless
         
        //PyEval_InitThreads(); // Initialize GIL

		Py_Initialize();

       
        object module = import("__main__");
        object name_space = module.attr("__dict__");

        // Setup global variables for script.
        //Only works for values set before scripts, is not dynamic!
        //name_space["KeyCombination"]=KeyCombination;
        
        
        // clear previous module namespace
        /*
        try {
            const char* code;
            object main_module = import("__main__");
            dict module_dict      = extract<dict>(main_module.attr("__dict__"));
            code = "a = 3";
            exec(code, module_dict, module_dict); // Should succeed
            code = "b = a";
            exec(code, module_dict, module_dict); // Should succeed

            module_dict.clear(); // deletes all variables
            module_dict["__builtins__"] = import("__builtin__");
            module_dict["__name__"]   = "__main__";
            module_dict["__doc__"]      = object();
            code = "c = b";
            exec(code, module_dict, module_dict); // Should fail, b not in namespace
        } catch (error_already_set&) {
            //Should end up here because of the last exec, complaining about 'b'
            PyErr_Print();
        }*/
        

        object result = exec_file(path.c_str(), name_space, name_space);

		std::cout << "Exiting Boost_Python" << std::endl;

    }
    catch(error_already_set const &)
    {
        PyErr_Print();
		/*
		boost::python::object sys(
  			boost::python::handle<>(PyImport_ImportModule("sys"))
		);
		boost::python::object err = sys.attr("stderr");
		std::string err_text = boost::python::extract<std::string>(err.attr("getvalue")());
		//MessageBox(0, err_text.c_str(), "Python Error", MB_OK);

		// Only Exit if a user, not if sysop debuging!
		if (isSysop == FALSE)
			exit(1);
             * */
    }


	// Unblock Signals	
	//sigprocmask(SIG_UNBLOCK, &signal_set, NULL);

	//do not call this, causes issue memory leaks in BOOST_PYTHON
	// can't run more anymore scripts if this is called!
    // Py_Finalize();
    return;
}
