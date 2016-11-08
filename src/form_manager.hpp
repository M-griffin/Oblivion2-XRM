#ifndef FORM_MANAGER_H
#define FORM_MANAGER_H

#include <string>
#include <vector>

/**
 * @class Form_Manager
 * @author Michael Griffin
 * @date 06/11/2016
 * @file form_manager.hpp
 * @brief Handles Form Creation and Management
 */
class Form_Manager
{
public:

    enum
    {
        FORM_DATA_CONFIG,
        FORM_DATA_SECURITY,
        FORM_DATA_USERS,
    };


    // Form Default Configuration Prompts
    //const std::string FORM_NAME = "newuser.form";
    //const std::string FORM_DATA = "secutiry, users";
        
    Form_Manager();
    ~Form_Manager();
    
    // Function Input Vector.
    //std::vector<std::function< void()> >                    m_form_data_functions;
    //std::vector<std::function< void(const std::string &)> > m_mod_functions;

};

#endif // FORM_MANAGER_H
