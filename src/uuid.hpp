#ifndef UUID_HPP
#define UUID_HPP

#include "encoding.hpp"

extern "C"
{
#ifdef _WIN32
// Winsock needed because Rpc includes windows.h.
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
        char uuid_string[37]={0};
        uuid_unparse_lower(uuid, uuid_string);
        uuid_clear(uuid);
#endif
        return std::string((char*)uuid_string);
    }
};

#endif
