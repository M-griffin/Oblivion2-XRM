#ifndef SECURITY_HPP
#define SECURITY_HPP

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>

class Security;
typedef boost::shared_ptr<Security> security_ptr;

/**
 * @class Security
 * @author Michael Griffin
 * @date 8/20/2016
 * @file security.hpp
 * @brief Structure for holding users Password and Salt hashs
 */
class Security
{
public:
   
    long iId;
    std::string sPasswordHash;
    std::string sSaltHash;
    std::string sChallengeQuestion;
    std::string sChallengeAnswerHash;

    explicit Security()
        : iId(-1)
        , sPasswordHash("")
        , sSaltHash("")
        , sChallengeQuestion("")
        , sChallengeAnswerHash("")
    { }
    ~Security() { }

};


#endif // SECURITY_HPP
