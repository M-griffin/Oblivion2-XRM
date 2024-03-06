#ifndef GROUPING_HPP
#define GROUPING_HPP

#include <memory>
#include <string>

class Grouping;
typedef std::shared_ptr<Grouping> group_ptr;

/**
 * @class Grouping
 * @author Michael Griffin
 * @date 03/05/2017
 * @file grouping.hpp
 * @brief Conference Grouping, Links Msg/File Areas to Conferences
 */
class Grouping
{
public:

    long        iId;
    long        iConferenceId;
    long        iAreaId;

    explicit Grouping()
        : iId(-1)
        , iConferenceId(-1)
        , iAreaId(-1)
    { }

    ~Grouping()
    { }

};

#endif // GROUPING_HPP

