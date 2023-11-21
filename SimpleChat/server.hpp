#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <thread>
#include <queue>
#include <functional>


namespace dsc {
    template<class t1, class t2>
    struct pair {
		t1 first;
		t2 second;
	};

    class server {
        enum request_type {
			DISCONNECT,
            CONNECT,
			SEND_PACKET,
            SEND_COMMAND
		};

        struct request {
            sf::TcpSocket *socket;
			request_type type;
            sf::Packet packet;
        };

        sf::TcpListener listener;
        std::vector<pair<sf::TcpSocket *, uint64_t>> clients;
        std::vector<sf::IpAddress> bannedIpAddresses;
        std::vector<sf::TcpSocket *> admins;

        sf::Uint16 listenPort;
        bool running;

        std::function<bool(sf::Packet, sf::TcpSocket *)> onReceivePacket;

        std::queue<std::string> commands;
        std::queue<request> requests;

        uint64_t privateKey;
        uint64_t publicKey;

        uint64_t modulus;
        uint64_t generator;


    public:

        server(sf::Uint16 = sf::Socket::AnyPort);
        std::string getIpAddress() const;
        sf::Uint16 getPort() const;
        bool isRunning() const;

        void getNewClients();
        void disconnectClient(sf::TcpSocket *, int32_t = -1);

        bool getPacket(sf::TcpSocket *, size_t);

        void broadcastPacket(sf::Packet &, sf::IpAddress, sf::Uint16);
        bool broadcastPacketBack(sf::Packet &, sf::TcpSocket *, uint8_t = 0);

        void disconnectAllClients();
        void sendDisconnectMessage(sf::TcpSocket *);

        void receivePackets();
        void manageRequests();
        void listenConsole();
        void sendCommand(std::string);

        bool banIpAddress(sf::IpAddress);
        bool unbanIpAddress(sf::IpAddress);
        bool isIpAddressBanned(sf::IpAddress);

        bool addAdmin(sf::TcpSocket *);
        bool removeAdmin(sf::TcpSocket *);
        bool isAdmin(sf::TcpSocket *);

        void setReceivePacketCallback(std::function<bool(sf::Packet, sf::TcpSocket *)>);

        bool start(sf::Uint16);
        bool stop();
        void run();
        void exit();
        void waitUntilEnd() const;
        ~server();
    };
}


