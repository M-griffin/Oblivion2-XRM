#ifndef MOD_BASE_HPP
#define MOD_BASE_HPP

#include "../model/config.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"
#include "../ansi_processor.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>
#include <string>


/**
 * @class ModBase
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_base.hpp
 * @brief Base Class for Module System
 */
class ModBase
{
public:

    virtual ~ModBase()
    {
        std::cout << "~ModBase." << std::endl;
    }
    virtual bool update(const std::string &character_buffer, const bool &is_utf8) = 0;
    virtual bool onEnter() = 0;
    virtual bool onExit()  = 0;

    ModBase(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
        : m_session_data(session_data)
        , m_config(config)
        , m_session_io(session_data)
        , m_ansi_process(ansi_process)
        , m_is_active(false)
    { }


    /**
     * @brief Gets the Default Color Sequence
     * @return 
     */
    std::string baseGetDefaultColor() 
    {
        return m_session_io.pipeColors(m_config->default_color_regular);
    }
    
    /**
     * @brief Gets the Default Input Color Sequence
     * @return 
     */
    std::string baseGetDefaultInputColor() 
    {
        return m_session_io.pipeColors(m_config->default_color_input);
    }
    
    /**
     * @brief Gets the Default Input Color Sequence
     * @return 
     */
    std::string baseGetDefaultInverseColor() 
    {
        return m_session_io.pipeColors(m_config->default_color_inverse);
    }

    /**
     * @brief Method for Adding outgoing text data to ansi processor
     *        Then delivering the data to the client
     * @param data
     */
    void baseProcessAndDeliver(std::string &data)
    {
        // Clear out attributes on new strings no bleeding of colors.
        std::string output = "\x1b[0m";
        output += std::move(data);
        m_ansi_process->parseAnsiScreen((char *)output.c_str());
        m_session_data->deliver(output);
    }

    // This holds session data passed to each session.
    // In modules we'll use the weak pointer so more clarity.
    session_data_ptr  m_session_data;
    config_ptr        m_config;
    SessionIO         m_session_io;
    ansi_process_ptr  m_ansi_process;
    bool              m_is_active;


    // All Data is saved to this buffer, which is then
    // Checked after each stage, onEnter, Update, onExit
};

typedef boost::shared_ptr<ModBase>	module_ptr;
typedef boost::weak_ptr<ModBase>	module_wptr;


#endif // MOD_BASE_HPP
