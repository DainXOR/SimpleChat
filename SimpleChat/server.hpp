#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <thread>
#include <queue>


namespace dsc {
    class server {
        sf::TcpListener listener;
        std::vector<sf::TcpSocket *> clientArray;
        unsigned short listenPort;
        bool running;

        std::thread *clientThread;
        std::thread *packetThread;
        std::thread *consoleThread;

        std::queue<std::string> consoleQueue;
    public:

        server(unsigned short);
        std::string getIpAddress();
        unsigned short getPort();

        void connectClients(std::vector<sf::TcpSocket *> *);
        void disconnectClient(sf::TcpSocket *, size_t);

        bool receivePacket(sf::TcpSocket *, size_t);

        void broadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);
        void broadcastPacketBack(sf::Packet &, sf::TcpSocket *);

        void disconnectAllClients();
        void sendDisconnectMessage(sf::TcpSocket *);

        void managePackets();
        void listenConsole();
        void sendCommand(std::string);
    };
}


