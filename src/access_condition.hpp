#ifndef ACCESS_CONDITION_HPP
#define ACCESS_CONDITION_HPP

#include "session_io.hpp"
#include "model/structures.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <vector>
#include <map>

class Users;
typedef boost::shared_ptr<Users> user_ptr;

/**
 * @class AccessCondition
 * @author Michael Griffin
 * @date 17/03/2017
 * @file access_condition.hpp
 * @brief Access Condition System handles Security Level and Flags
 */
class AccessCondition
{
public:
    AccessCondition()  { };
    ~AccessCondition() { };
    
    /**
     * @brief Parse Code Map and Test Secutiry and AR Flags.
     * @param code_map
     * @param user
     * @return 
     */
    bool parseCodeMap(const std::vector<MapType> &code_map, user_ptr user);

    /**
     * @brief Parse ASC Strings then test User Flags
     * @param acs_string
     * @param user
     * @return 
     */
    bool parseAcsString(std::string &acs_string, user_ptr user);

    // Using Session IO for Code Mapping
    SessionIO  m_session_io;

    // note update to secutiry expressions
    // start with NOT s255 then test for s255
    // start with all not, but all normal will get caught and never pass through.
    // seperate expressons with | or. and ( )
    const std::string ACS_EXPRESSION = {"([|]{1}[0-9]{2})|([|]{1}[X][Y][0-9]{4})|"
                                        "([|]{1}[A-Z]{1,2}[0-9]{1,2})|([|]{1}[A-Z]{2})|"
                                        "([%]{2}[\\w]+[.]{1}[\\w]{3})|([%]{1}[A-Z]{2})|"
                                        "([%]{1}[0-9]{2})"
                                       };

};

#endif // ACCESS_CONDITION_HPP
