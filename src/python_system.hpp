#ifndef PYTHON_SYSTEM_HPP
#define PYTHON_SYSTEM_HPP

#ifdef HAVE_PYTHON
#include "state_base.hpp"
#include "menu_base.hpp"

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

class Directory;
typedef std::shared_ptr<Directory> directory_ptr;

/**
 * @class PythonSystem
 * @author Michael Griffin
 * @date 18/06/22
 * @file python_system.hpp
 * @brief Python Interperter via PyBind11 interface.
 */
class PythonSystem
    : public StateBase
{
public:
    explicit PythonSystem(session_data_ptr session_data);
    ~PythonSystem();

    virtual void update(const std::string &character_buffer, const bool &is_utf8) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    virtual std::string getStateID() const override
    {
        return m_stateID;
    }
    
    std::thread createThread(const std::string &path);
    
    static const std::string m_stateID;
    directory_ptr m_directory;
    std::thread m_script_thread;
    
    // Run an external python script.
    void pyRunScript(const std::string &script);
};

#endif // HAVE_PYTHON
#endif // PYTHON_SYSTEM_HPP
