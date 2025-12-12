#pragma once

#include <SFML/Network.hpp>

#include <vector>
#include <string>
#include <queue>

#include "utils.hpp"

namespace dsc {
	class simple_server {
	public:
		using full_packet = duc::pair_any<sf::Packet, sf::TcpSocket*>;

		sf::TcpListener listener;
		sf::Uint16 listenPort;

		std::vector<sf::TcpSocket *> clients;
		std::queue<full_packet> packets;

		bool connectionState = false;

	public:
		simple_server(sf::Uint16 = sf::Socket::AnyPort);
		std::string getIpAddress() const;
		sf::Uint16 getPort() const;
		bool isRunning() const;

		void connectNewClients();
		void disconnectClients();

		void receivePackets();
		void broadcastPackets();

		bool run();
		void waitUntilExit() const;
	};
}



