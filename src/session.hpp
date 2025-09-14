#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include "sdl2_net/SDL_net.hpp"

class Session {
public:
    Session(TCPsocket socket, int nodeNumber)
        : socket(socket), nodeNumber(nodeNumber), active(true) {}

    // Non-copyable
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    // Movable
    Session(Session&& other) noexcept
        : socket(other.socket),
          nodeNumber(other.nodeNumber),
          active(other.active) {
        other.socket = nullptr;
        other.nodeNumber = -1;
        other.active = false;
    }

    Session& operator=(Session&& other) noexcept {
        if (this != &other) {
            close();
            socket = other.socket;
            nodeNumber = other.nodeNumber;
            active = other.active;

            other.socket = nullptr;
            other.nodeNumber = -1;
            other.active = false;
        }
        return *this;
    }

    ~Session() {
        close();
    }

    bool isActive() const {
        return active;
    }

    TCPsocket getSocket() const {
        return socket;
    }

    int getNodeNumber() const {
        return nodeNumber;
    }

    std::string receive() {
        char buffer[512];
        int received = SDLNet_TCP_Recv(socket, buffer, sizeof(buffer) - 1);
        if (received <= 0) {
            active = false;
            return {};
        }

        buffer[received] = '\0';
        return std::string(buffer);
    }

    void send(const std::string& message) {
        if (!active) return;
        SDLNet_TCP_Send(socket, message.c_str(), message.length());
    }

    void close() {
        if (socket) {
            SDLNet_TCP_Close(socket);
            socket = nullptr;
        }
        active = false;
    }

private:
    TCPsocket socket;
    int nodeNumber;
    bool active;
};

#endif
