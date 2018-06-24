#ifndef UUID_HPP
#define UUID_HPP

extern "C"
{
#ifdef _WIN32
// Winsock needed becasue Rpc includes windows.h.
    #include <winsock2.h>
    #include <Rpc.h>
#else
    #include <uuid/uuid.h>
#endif
}

#include <string>
#include <cstring>

class Uuid
{
    
public:    

    std::string createUuidString()
    {
#ifdef _WIN32
        UUID uuid;
        UuidCreate (&uuid);

        unsigned char *id_string;
        UuidToStringA(&uuid, &id_string);

        std::string uuid_string((char*)id_string);
        RpcStringFreeA ( &id_string );
#else
        uuid_t uuid;
        uuid_generate_random(uuid);
        char uuid_string[50];
        memset(&uuid_string, 0, strlen(uuid_string));
        uuid_unparse (uuid, uuid_string);
#endif
        return uuid_string;
    }
};

#endif
