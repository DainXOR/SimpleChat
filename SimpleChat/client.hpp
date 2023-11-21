#pragma once
#include <SFML\Network.hpp>
#include <functional>

namespace dsc {
    class client {
    public:
        sf::TcpSocket socket;
        sf::Packet lastPacket;

        bool connectionState = false;

    public:
        void connect(const char *, unsigned short);
        void disconnect();

        bool isConnected();

        void receivePacket(sf::TcpSocket *);
        void sendPacket(sf::Packet &);
        void getLatestPacket(sf::Packet &) const;
        void run();
    };
}


