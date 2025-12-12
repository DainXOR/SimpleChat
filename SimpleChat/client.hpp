#pragma once
#include <SFML/Network.hpp>

#include <functional>
#include <queue>

namespace dsc {
    /*
    * enum packet_info {
		CLIENT		= 0b1000,
		SERVER		= 0b0100,
		OTHER		= 0b0010,
		UNKNOWN		= 0b0001,

		INCOMING	= 0b000010,
		OUTGOING	= 0b000001,

		MESSAGE		= 0b000000100,
		COMMAND		= 0b000000010,
		REQUEST		= 0b000000001,

	};

	packet_info operator|(packet_info, packet_info);
	packet_info operator|=(packet_info&, packet_info);

    packet_info operator|(packet_info left, packet_info right) {
        return static_cast<packet_info>(static_cast<int>(left) | static_cast<int>(right));
    }
    packet_info operator|=(packet_info& left, packet_info right) {
        return left = left | right;
    }

    */


    class client {
    public:
        sf::TcpSocket socket;
        std::queue<sf::Packet> packets;

        uint64_t privateKey;
        uint64_t publicKey;
        uint64_t modulus;

        bool connectionState = false;

    public:
        client();

        bool connect(const char *, unsigned short);
        bool disconnect();

        bool isConnected() const;

        void receivePackets();
        bool sendPacket(sf::Packet &);
        void handlePackets();
        void listenConsole();

        std::string encrypt(std::string, size_t);
        std::string decrypt(std::string, size_t);

        bool run();
        void waitUntilExit() const;
    };
}


