#pragma once
#include <SFML/Network.hpp>

class client {
    sf::TcpSocket socket;
    sf::Packet lastPacket;

    bool isConnected = false;
public:
    client();
    void connect(const char *, unsigned short);
    void receivePackets(sf::TcpSocket *);
    void sendPacket(sf::Packet &);
    void run();
};

