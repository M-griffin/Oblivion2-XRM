#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

class Network;
typedef boost::shared_ptr<Network> network_ptr;

/**
 * @class Network
 * @author Michael Griffin
 * @date 03/05/2017
 * @file network.hpp
 * @brief Network Formats: Type - Fido, IP Address
 */
class Network
{
public:
  
    long        iId;
    std::string sName;
    std::string sType;
    std::string sAddress;
    
    explicit Network()
        : iId(-1)
        , sName("")
        , sType("")
        , sAddress("")
    { }
    
    ~Network()
    { }

};

#endif // NETWORK_HPP

