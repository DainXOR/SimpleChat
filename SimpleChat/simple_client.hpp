#pragma once

#include <SFML/Network.hpp>

#include <queue>

#include "abstract_client.hpp"

namespace dsc {
	
	class simple_client: public abstract_client {
	public:
		sf::TcpSocket socket;
		std::queue<sf::Packet> packetsToSend;
		std::queue<sf::Packet> receivedPackets;

		bool connectionState = false;

	public:
		simple_client();

		bool connect(const char *, unsigned short);
		bool disconnect();

		bool isConnected() const;

		sf::TcpSocket &getSocket();
		std::queue<sf::Packet>& getPacketsToSend();
		std::queue<sf::Packet>& getReceivedPackets();

		bool sendPacket(sf::Packet &);

		void handleReceivedPackets();
		void handleToSendPackets();

		sf::Packet handleSendingPacket(sf::Packet);
		sf::Packet handleReceivingPacket(sf::Packet);

		void handlePackets();
		void receivePackets();
		void listenConsole();

		bool run();
		void waitUntilExit() const;
	};
}


