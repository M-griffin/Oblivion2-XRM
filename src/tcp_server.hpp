#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "model-sys/config.hpp"
#include "session.hpp"
#include "logging.hpp"

#include "sdl2_net/SDL_net.hpp"
#include "libSqliteWrapped.h"

class TcpServer {
public:
    explicit TcpServer(Uint16 port, int maxSessions = 16)
        : serverSocket(nullptr)
          , socketSet(nullptr)
          , port(port)
          , maxSessions(maxSessions)
          , isRunning(false)
          , userDatabase(USERS_DATABASE, &databaseLog) {
        for (int i = 1; i <= maxSessions; ++i) {
            availableNodes.insert(i);
        }
    }

    ~TcpServer() {
        stop();
    }

    bool start() {
        if (SDLNet_Init() < 0) {
            std::cerr << "[Server] SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
            return false;
        }

        if (SDLNet_ResolveHost(&serverIP, nullptr, port) == -1) {
            std::cerr << "[Server] SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
            return false;
        }

        serverSocket = SDLNet_TCP_Open(&serverIP);
        if (!serverSocket) {
            std::cerr << "[Server] SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
            return false;
        }

        socketSet = SDLNet_AllocSocketSet(maxSessions + 1);
        if (!socketSet) {
            std::cerr << "[Server] SDLNet_AllocSocketSet failed: " << SDLNet_GetError() << std::endl;
            return false;
        }

        SDLNet_TCP_AddSocket(socketSet, serverSocket);
        isRunning = true;

        std::cout << "[Server] Listening on port " << port << std::endl;
        return true;
    }

    void run() {
        if (!isRunning) {
            std::cerr << "[Server] Cannot run server; not initialized.\n";
            return;
        }

        // Add shutdown flag WFC or soemthing.
        while (isRunning) {
            // Can check for Events to display to users etc... or execute.


            // Accept new clients
            TCPsocket newClient = SDLNet_TCP_Accept(serverSocket);
            if (newClient) {
                if (sessions.size() < static_cast<size_t>(maxSessions) && !availableNodes.empty()) {
                    int nodeId = *availableNodes.begin();
                    availableNodes.erase(nodeId);

                    SDLNet_TCP_AddSocket(socketSet, newClient);
                    sessions.emplace_back(newClient, nodeId, config);
                    std::cout << "[Server] New client connected! Node #" << nodeId
                            << ", Total: " << sessions.size() << "\n";
                } else {
                    std::cerr << "[Server] Max clients reached or no available node IDs. Rejecting.\n";
                    SDLNet_TCP_Close(newClient);
                }
            }

            const int numReady = SDLNet_CheckSockets(socketSet, 0);
            if (numReady < 0) {
                std::cerr << "[Server] SDLNet_CheckSockets failed: " << SDLNet_GetError() << "\n";

                const char* error = SDLNet_GetError();
                if (strstr(error, "invalid socket")) {
                    std::cerr << "Invalid socket detected, removing it from the set." << "\n";
                    // Remove the problematic socket from the set
                    // Iterate through all sockets to find the invalid one
                    for (auto it = sessions.begin(); it != sessions.end(); ) {
                        TCPsocket clientSocket = it->getSocket();
                        if (!SDLNet_SocketReady(clientSocket)) {
                            // Remove the invalid socket from the set
                            SDLNet_TCP_DelSocket(socketSet, clientSocket);
                            SDLNet_TCP_Close(clientSocket);
                            int nodeId = it->getNodeNumber();
                            availableNodes.insert(it->getNodeNumber());

                            // Remove the client from the list
                            it = sessions.erase(it);
                            std::cerr << "[Server] Removed invalid socket for Node #" << nodeId << "\n";
                        } else {
                            ++it;
                        }
                    }
                } else {
                    std::cerr << "Critical error:" << error << "\n";
                    // Retry next iteration.
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            for (auto it = sessions.begin(); it != sessions.end();) {
                TCPsocket sock = it->getSocket();

                if (SDLNet_SocketReady(sock)) {
                    std::string msg = it->receive();
                    if (msg.empty()) {
                        int nodeId = it->getNodeNumber();
                        std::cout << "[Server] Client node #" << nodeId << " disconnected.\n";

                        SDLNet_TCP_DelSocket(socketSet, sock);
                        it = sessions.erase(it);

                        // Reclaim the node number
                        availableNodes.insert(nodeId);

                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }

                    std::cout << "[Server][Node #" << it->getNodeNumber() << "] Received: " << msg << "\n";
                    it->send("Server received: " + msg);
                }

                ++it;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void stop() {
        isRunning = false;

        for (auto &session: sessions) {
            SDLNet_TCP_DelSocket(socketSet, session.getSocket());
            session.close();
        }
        sessions.clear();
        availableNodes.clear();

        if (serverSocket) {
            SDLNet_TCP_DelSocket(socketSet, serverSocket);
            SDLNet_TCP_Close(serverSocket);
        }

        if (socketSet)
            SDLNet_FreeSocketSet(socketSet);

        SDLNet_Quit();
        std::cout << "[Server] Server stopped.\n";
    }

private:
    TCPsocket serverSocket;
    IPaddress serverIP{};
    SDLNet_SocketSet socketSet;
    std::vector<Session> sessions;
    std::set<Uint16> availableNodes;
    Uint16 port;
    Uint16 maxSessions;
    bool isRunning;

    Config config;

    SQLW::Database userDatabase;
    SQLW::StderrLog databaseLog;
};

#endif
