#pragma once
#include <SFML/Network.hpp>

class server {
    sf::TcpListener listener;
    std::vector<sf::TcpSocket *> clientArray;

    unsigned short listenPort;

public:
    server(unsigned short);
    void connectClients(std::vector<sf::TcpSocket *> *);
    void disconnectClient(sf::TcpSocket *, size_t);

    void receivePacket(sf::TcpSocket *, size_t);

    void broadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);

    void managePackets();
    void run();
};

