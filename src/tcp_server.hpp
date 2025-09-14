#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>

#include "sdl2_net/SDL_net.hpp"
#include "logging.hpp"
#include "session.hpp"


class TcpServer {
public:
    TcpServer(Uint16 port, int maxClients = 16)
        : serverSocket(nullptr), socketSet(nullptr), port(port),
          maxClients(maxClients), isRunning(false) {
        for (int i = 1; i <= maxClients; ++i) {
            availableNodes.insert(i);
        }
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

        socketSet = SDLNet_AllocSocketSet(maxClients + 1);
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

        while (isRunning) {
            // Accept new clients
            TCPsocket newClient = SDLNet_TCP_Accept(serverSocket);
            if (newClient) {
                if (clients.size() < static_cast<size_t>(maxClients) && !availableNodes.empty()) {
                    int nodeId = *availableNodes.begin(); // Take lowest available node number
                    availableNodes.erase(nodeId);

                    SDLNet_TCP_AddSocket(socketSet, newClient);
                    clients.emplace_back(newClient, nodeId);
                    std::cout << "[Server] New client connected! Node #" << nodeId
                              << ", Total: " << clients.size() << "\n";
                } else {
                    std::cerr << "[Server] Max clients reached or no available node IDs. Rejecting.\n";
                    SDLNet_TCP_Close(newClient);
                }
            }

            int numReady = SDLNet_CheckSockets(socketSet, 50);
            if (numReady < 0) {
                std::cerr << "[Server] SDLNet_CheckSockets failed: " << SDLNet_GetError() << "\n";
                continue;
            }

            for (auto it = clients.begin(); it != clients.end(); ) {
                TCPsocket sock = it->getSocket();

                if (SDLNet_SocketReady(sock)) {
                    std::string msg = it->receive();
                    if (msg.empty()) {
                        int nodeId = it->getNodeNumber();
                        std::cout << "[Server] Client node #" << nodeId << " disconnected.\n";

                        SDLNet_TCP_DelSocket(socketSet, sock);
                        it = clients.erase(it);

                        // Reclaim the node number
                        availableNodes.insert(nodeId);
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

        for (auto& session : clients) {
            SDLNet_TCP_DelSocket(socketSet, session.getSocket());
            session.close();
        }
        clients.clear();
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

    ~TcpServer() {
        stop();
    }

private:
    TCPsocket serverSocket;
    IPaddress serverIP;
    SDLNet_SocketSet socketSet;
    std::vector<Session> clients;
    std::set<int> availableNodes;
    Uint16 port;
    int maxClients;
    bool isRunning;
};

#endif
