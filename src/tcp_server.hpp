#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <set>

#include "model-sys/config.hpp"
#include "model-sys/structures.hpp"

#include "tcp_session.hpp"
#include "sdl2_net/SDL_net.hpp"
#include "libSqliteWrapped.h"

class TCPServer {
    TCPsocket serverSocket = nullptr;
    IPaddress serverIP{};
    SDLNet_SocketSet socketSet = nullptr;

    std::list<TCPSession> sessions;
    std::set<Uint16> availableNodes;
    bool isRunning = false;

    SQLW::Database coreDatabase;
    SQLW::StderrLog databaseLog;

    TCPServer()
        : coreDatabase(CORE_DATABASE, &databaseLog) {
    }

public:
    static TCPServer &getInstance() {
        static TCPServer instance;
        return instance;
    }

    TCPServer(const TCPServer &) = delete;
    TCPServer &operator=(const TCPServer &) = delete;
    TCPServer(const TCPServer &&) = delete;
    TCPServer &operator=(const TCPServer &&) = delete;

    bool start(const Uint16 telnetPort, const Uint16 maxSessions) {
        for (Uint16 i = 1; i <= maxSessions; ++i) {
            availableNodes.insert(i);
        }

        if (SDLNet_Init() < 0) {
            std::cerr << "[Server] SDLNet_Init failed: " << SDLNet_GetError() << "\n";
            return false;
        }

        if (SDLNet_ResolveHost(&serverIP, nullptr, telnetPort) < 0) {
            std::cerr << "[Server] ResolveHost failed: " << SDLNet_GetError() << "\n";
            return false;
        }

        serverSocket = SDLNet_TCP_Open(&serverIP);
        if (!serverSocket) {
            std::cerr << "[Server] TCP_Open failed: " << SDLNet_GetError() << "\n";
            return false;
        }

        socketSet = SDLNet_AllocSocketSet(maxSessions + 1);
        if (!socketSet) {
            std::cerr << "[Server] AllocSocketSet failed\n";
            return false;
        }

        SDLNet_TCP_AddSocket(socketSet, serverSocket);
        isRunning = true;

        std::cout << "[Server] Listening on port " << telnetPort << "\n";
        return true;
    }

    void run(Config &config, const Uint16 maxSessions) {
        while (isRunning) {
            // FIRST: Cleanup any already inactive sessions
            for (auto it = sessions.begin(); it != sessions.end();) {
                if (!it->isActive()) {
                    cleanupSession(it);
                } else {
                    ++it;
                }
            }

            // Wait for socket activity
            const int ready = SDLNet_CheckSockets(socketSet, 10);
            if (ready < 0) {
                std::cerr << "[Server] SDLNet_CheckSockets failed: "
                        << SDLNet_GetError() << "\n";
                rebuildSocketSet(maxSessions);
                continue;
            }

            // Accept new clients (ONLY if ready)
            if (SDLNet_SocketReady(serverSocket)) {
                TCPsocket client = SDLNet_TCP_Accept(serverSocket);
                if (client) {
                    if (!availableNodes.empty() && sessions.size() < maxSessions) {
                        Uint16 nodeId = *availableNodes.begin();
                        availableNodes.erase(nodeId);

                        SDLNet_TCP_AddSocket(socketSet, client);
                        sessions.emplace_back(client, nodeId, config, coreDatabase);
                        sessions.back().startSession();

                        std::cout << "[Server] Client connected (Node "
                                << nodeId << ")\n";
                    } else {
                        SDLNet_TCP_Close(client);
                    }
                }
            }

            // Handle sessions
            for (auto it = sessions.begin(); it != sessions.end();) {
                auto &session = *it;
                TCPsocket sock = session.getSocket();

                if (!session.isActive()) {
                    cleanupSession(it);
                    continue;
                }

                if (SDLNet_SocketReady(sock)) {
                    ByteBuffer data = session.receive();

                    // TCP closed or error
                    if (data.empty()) {
                        std::cout << "[Server] Client node " << session.getNodeNumber()
                                << " disconnected (recv 0 or error)\n";
                        session.hangup();
                        cleanupSession(it);
                        continue;
                    }

                    // Forward valid data to session for processing
                    session.handleIncomingData(data);
                } else {
                    // Poll timers for active session
                    session.pollTimers();
                }

                ++it;
            }
        }
    }

    void stop() {
        isRunning = false;

        for (auto &s: sessions) {
            s.hangup();
            SDLNet_TCP_DelSocket(socketSet, s.getSocket());
            s.close();
        }

        sessions.clear();
        availableNodes.clear();

        if (serverSocket) {
            SDLNet_TCP_DelSocket(socketSet, serverSocket);
            SDLNet_TCP_Close(serverSocket);
        }

        if (socketSet) {
            SDLNet_FreeSocketSet(socketSet);
        }

        SDLNet_Quit();
    }

    SQLW::Database &getDatabase() {
        return coreDatabase;
    }

private:
    void cleanupSession(std::list<TCPSession>::iterator &it) {
        if (it == sessions.end()) {
            return;
        }

        Uint16 nodeId = it->getNodeNumber();
        TCPsocket sock = it->getSocket();

        if (sock) {
            SDLNet_TCP_DelSocket(socketSet, sock);
            it->close();
        }

        availableNodes.insert(nodeId);
        it = sessions.erase(it);

        std::cout << "[Server] Client node " << nodeId << " fully disconnected\n";
    }

    void rebuildSocketSet(Uint16 maxSessions) {
        SDLNet_FreeSocketSet(socketSet);
        socketSet = SDLNet_AllocSocketSet(maxSessions + 1);

        SDLNet_TCP_AddSocket(socketSet, serverSocket);
        for (auto &s: sessions) {
            if (s.isActive()) {
                SDLNet_TCP_AddSocket(socketSet, s.getSocket());
            }
        }
    }
};

#endif
