#ifndef ONELINERS_HPP
#define ONELINERS_HPP

class oneliners
{
public:
    oneliners() 
        : iId(-1)
        , iUserId(-1)
        , text("")
        , userName("")
        , userInitials("")
        , datePosted(0)
    {}
    ~oneliners();

    long iId;
    long iUserId;        
    std::string text;
    std::string userName;
    std::string userInitials;
    std::time_t datePosted;

};

#endif // ONELINERS_HPP
