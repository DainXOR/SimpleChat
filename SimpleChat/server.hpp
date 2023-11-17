#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

namespace dsc {
    class server {
        sf::TcpListener listener;
        std::vector<sf::TcpSocket *> clientArray;

        unsigned short listenPort;

    public:
        server(unsigned short);
        std::string getIpAddress();
        unsigned short getPort();

        void connectClients(std::vector<sf::TcpSocket *> *);
        void disconnectClient(sf::TcpSocket *, size_t);

        void receivePacket(sf::TcpSocket *, size_t);

        void broadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);
        void broadcastPacketBack(sf::Packet &, sf::TcpSocket *);

        void managePackets();
        void run();
    };
}


