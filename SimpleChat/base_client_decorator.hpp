#pragma once

#include <SFML/Network.hpp>

#include <queue>

#include "abstract_client.hpp"

namespace dsc {

	class base_client_decorator: public abstract_client {
		abstract_client *decoratedClient;

	public:
		base_client_decorator(abstract_client *);
		~base_client_decorator();

		bool connect(const char *, unsigned short);
		bool disconnect();

		bool isConnected() const;

		abstract_client *getClient();
		sf::TcpSocket &getSocket();
		std::queue<sf::Packet>& getPacketsToSend();
		std::queue<sf::Packet>& getReceivedPackets();

		bool sendPacket(sf::Packet &);

		void handleReceivedPackets();
		void handleToSendPackets();

		sf::Packet handleSendingPacket(sf::Packet);
		sf::Packet handleReceivingPacket(sf::Packet);

		void receivePackets();
		void handlePackets();
		void listenConsole();

		bool run();
		void waitUntilExit() const;

	};
}


