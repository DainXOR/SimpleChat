#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <thread>
#include <queue>
#include <functional>


namespace dsc {
    class server {
        sf::TcpListener listener;
        std::vector<sf::TcpSocket *> clientArray;
        unsigned short listenPort;
        bool running;

        std::function<bool(sf::Packet, sf::TcpSocket *)> onRecivePacket;

        //std::thread *clientThread;
        //std::thread *packetThread;
        //std::thread *consoleThread;

        std::queue<std::string> consoleQueue;
    public:

        server(unsigned short = sf::Socket::AnyPort);
        std::string getIpAddress();
        unsigned short getPort();
        bool isRunning() const;

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

        void setRecivePacketCallback(std::function<bool(sf::Packet, sf::TcpSocket *)>);

        void run();
    };
}


