#ifndef MESSAGE_AREA_HPP
#define MESSAGE_AREA_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

class MessageArea;
typedef boost::shared_ptr<MessageArea> message_area_ptr;

/**
 * @class MessageArea
 * @author Michael Griffin
 * @date 03/05/2017
 * @file message_area.hpp
 * @brief Message Area Model
 */
class MessageArea
{
public:

/*
          Base Name           - Name of message base.
          Access ACS          - ACS required to get into message base.
          Post ACS            - ACS required to post in message base.
          Allow Anonymous     - Can users post anonymously in this
                                message base?
          Sponsor             - Pseudo-SysOp. Can delete all messages,
                                read all messages in the current base.
          Origin Line         - Line BBS adds to the end of a
                                echomailed message. Usually showed BBS
                                name and #. Pipe codes supported.
          Echomail directory  - Directory where echomail MSG files
                                will be held, imported, and exported
                                from.
          Netmail             - Should this base support private
                                netmail through Fidonet mail. For this
                                option to work, you must compile a copy
                                of your nodelist and place it in your
                                Echomail directory. Compile the
                                nodelist as filename nodelist.dat, and
                                using BINKLEY6 format. The reason you
                                must compile the nodelist in the
                                echomail directory is for the nodelist
                                lookup function of the BBS. The BBS
                                will allow users to search the nodelist
                                for the node that they wish to send
                                netmail to.
          Node                - Assigned to you by your network
                                moderator. Basically shows location.
                                Form varies, but usually its X:XXX/XXX.
          QWKName             - Shortened name of the message base to
                                be displayed to users in their offline
                                readers. Most offline readers cannot
                                support message base names over 10-12
                                spaces. Use this option so that users
                                can better recognize which base they
                                are in from their reader.
          Autodelete          - How many total messages do you want in
                                the message base. When the # of
                                messages in the base is equal to the
                                number in this field, 5 messages will
                                be erased. This keeps your message base
                                files from growing increasingly large.
          Real names          - Should the user's real name be used in
                                place of a handle in this message base?
          Base filename       - This is an up to 8 character filename
                                that the BBS will use to store your
                                messages in.
          Mandatory           - When this flag is set to Yes, all the
                                messages in this base will be forced.
                                Users must read these messages. Use
                                message bases like this for news.
          Allow private       - Should users be able to post private
                                messages in this base. Use this flag if
                                the network you're in supports private
                                echomailing.
*/        
    long        iId;
    std::string sName;
    std::string sAcsAccess;
    std::string sAcsPost;
    bool        bAnonymous;
    std::string sSponsor;    
    std::string sOriginLine;
    std::string sFidoPath;
    long        iNetworkId;
    std::string sQwkName;
    long        iMaxMessages;
    bool        bRealName;
    std::string sLinkname;
    bool        bRequired;
    bool        bPrivate;
    bool        bNetmail;
    long        iSortOrder;
    
    explicit MessageArea()
        : iId(-1)
        , sName("")
        , sAcsAccess("")
        , sAcsPost("")
        , bAnonymous(false)
        , sSponsor("")
        , sOriginLine("")
        , sFidoPath("")
        , iNetworkId(-1)
        , sQwkName("")
        , iMaxMessages(0)
        , bRealName(false)
        , sLinkname("")
        , bRequired(false)
        , bPrivate(false)
        , bNetmail(false)
        , iSortOrder(-1)
    { }
    
    ~MessageArea()
    { }

};

#endif // MESSAGE_AREA_HPP

