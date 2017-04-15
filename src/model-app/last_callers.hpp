#ifndef LAST_CALLERS_HPP
#define LAST_CALLERS_HPP

#include <string>

/**
 * @class LastCallers
 * @author Michael Griffin
 * @date 10/04/2017
 * @file last_callers.hpp
 * @brief Model for Last Callers to System
 */
class LastCallers
{
public:
    LastCallers()
    {}
    
    ~LastCallers()
    {}
    
    long iId;
    long iUserId;    
    std::time_t timeOn;
    int  iNodeNumber;
    
    // * Bit Flags
    // 1 NewUser
    // 2 Sysop
    // 3 Available for Chat
    
    
    Reserved : Array[1..8] Of Byte;
   


};

#endif // LAST_CALLERS_HPP
