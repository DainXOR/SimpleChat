#pragma once
#include <SFML/Network.hpp>
#include <queue>

namespace dsc {
    class client {
    public:
        sf::TcpSocket socket = {};
        sf::Packet lastPacket = {};
        std::queue<sf::Packet> packets = {};

        bool connectionState = false;

    public:
        bool connect(const char *, unsigned short);
        void disconnect();

        bool isConnected() const;

        void receivePackets(sf::TcpSocket *);
        bool sendPacket(sf::Packet &);

        bool hasPackets() const;
        sf::Packet getLatestPacket() const;
        sf::Packet nextPacket();

        std::string getIpAddress() const;
        unsigned short getPort() const;

        void run();
    };
}


