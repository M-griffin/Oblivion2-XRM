#include "communicator.hpp"

#include "data/struct_compat.hpp"

#include <iostream>
#include <string>
#include <cstring>

/**
 * @brief  Global Singleton Instance, needed to initalize the class.
 */
Communicator* Communicator::m_globalInstance = nullptr;

/**
 * @brief Default Constructors required.
 * @return
 */
Communicator::Communicator()
{
    std::cout << "Communicator" << std::endl;
}

Communicator::~Communicator()
{
    std::cout << "~Communicator" << std::endl;
}
