
#ifndef BROAD_CASTER_HPP
#define BROAD_CASTER_HPP

/*
 *
 * Need to work out issue, can't have session, then sessiondata populated in here.
 * When it links there is no way to cleany shutdown both!
 *
 * Possibly, if and only if all sessions are disconnect first!  then
 * we proceddure with the shutdown!  look into this more!
 */

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <iostream>
#include <string>
#include <set>

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

class Session;
typedef boost::shared_ptr<Session> session_ptr;

/**
 * @class BroadCaster
 * @author Michael Griffin
 * @date 15/08/2015
 * @file boad_caster.hpp
 * @brief Main Channel of Communications between Sessions
 * Also Very Import, keeps handle on Active Sessions and keeps smart pointers alive!
 */
class BroadCaster
{
public:
    BroadCaster()
    {
        std::cout << "Global BroadCaster Created." << std::endl;
    }
    ~BroadCaster();

    void join(session_ptr session);
    void leave(int node_number);
    void deliver(std::string msg);

    int connections();
    void shutdown();

private:
    //std::set<session_data_ptr> m_participants;
    std::set<session_ptr> m_sessions;
};

typedef boost::shared_ptr<BroadCaster>	board_caster_ptr;
typedef boost::weak_ptr<BroadCaster>	board_caster_wptr;

#endif // CHAT_ROOM_HPP
