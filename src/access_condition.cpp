#include "access_condition.hpp"


#include <string>
#include <iostream>
#include <sstream>


AccessCondition::AccessCondition()
{
}

AccessCondition::~AccessCondition()
{
}


// Pass data strings to m_session_io.parseToCodeMap()
// Return and check code map for access controls and flags.

// Build methods to to parse secrity, flags, and others from obv2.
// methods should return boolean tests to validate user access.
