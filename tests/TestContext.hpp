#pragma once
/*=====================================================================
 *  TestContext.hpp – lightweight, real Context for the menu unit tests
 *
 *  The objects are created with *dummy* data only (null socket,
 *  in‑memory SQLite, default Config).  No virtual inheritance,
 *  no smart‑pointer gymnastics – just plain members that match the
 *  signatures the production code expects.
 *===================================================================*/

#include <memory>
#include <string>
#include <cassert>
#include <iostream>
#include <optional>
#include <cstddef>

/* -----------------------------------------------------------------
 *  Production headers – these are the same ones the real server uses
 * ----------------------------------------------------------------- */
#include "../src/model-sys/context.hpp"                // real Context class
#include "../src/tcp_socket_base.hpp"       // TcpSocketBase
#include "../src/tcp_session_wrapper.hpp"   // TcpSessionWrapper
#include "../src/tcp_telnet.hpp"            // TcpTelnet
#include "../src/io_session.hpp"            // IoSession
#include "../src/screen_ansi_proc.hpp"      // ScreenAnsiProc
#include "../src/io_common.hpp"            // IoCommon
#include "../src/model-sys/users.hpp"      // Users
#include "../src/model-sys/config.hpp"     // Config
#include "../sqlitewrap/cmake-build-debug/include/libSqliteWrapped.h" // SQLW::Database
#include "../src/sdl2_net/SDL_net.hpp"     // defines TCPsocket

/* -----------------------------------------------------------------
 *  TestContext – owns *real* objects, binds them into a Context
 * ----------------------------------------------------------------- */
class TestContext {
public:
    TestContext()
        : cfg{}                                      // default Config
        , db(":memory:", nullptr, 0)                 // SQLite – in‑memory DB
        , dummySocket(nullptr, 0, cfg)               // real TcpSocketBase, dummy socket
        , writer(dummySocket)                        // TcpSessionWrapper wraps dummySocket
        , telnet(writer)                             // TcpTelnet wraps writer
        , ioSession(writer, common)                  // IoSession needs writer + common
    {
        /*  Bind everything into the real Context.  The bind signature
         *  is exactly the same as the production code, therefore any
         *  class that expects a Context& (e.g. MenuBase) will see a
         *  perfectly valid object.
         */
        ctx.bind(writer, telnet, user, ansi, common, ioSession,
                 cfg, db);
    }

    /*  Helper used by the unit‑tests – they need a Context&          */
    Context& get() { return ctx; }

private:
    /* -----------------------------------------------------------------
     *  Dummy socket handle – SDL2‑net expects a TCPsocket, which is a
     *  pointer type.  `nullptr` is a legal, inert value.
     * ----------------------------------------------------------------- */
    static constexpr TCPsocket dummySock = nullptr;   // no real network activity

    /* -----------------------------------------------------------------
     *  All objects that the menu engine uses
     * ----------------------------------------------------------------- */
    Config                     cfg;          // configuration (default values)
    SQLW::Database             db;           // SQLite – in‑memory database

    TcpSocketBase              dummySocket;  // constructed with dummySock
    TcpSessionWrapper          writer;       // wraps dummySocket
    TcpTelnet                  telnet;       // wraps writer
    Users                      user;         // empty user record
    ScreenAnsiProc             ansi;         // does nothing heavy
    IoCommon                   common;       // utility class
    IoSession                  ioSession;    // needs writer + common

    Context                    ctx;          // the object that is passed around
};