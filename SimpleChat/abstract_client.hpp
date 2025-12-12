#pragma once
#include <SFML/Network.hpp>

#include <queue>


namespace dsc {
	class abstract_client {
	public:
		virtual bool connect(const char *, unsigned short) = 0;
		virtual bool disconnect() = 0;

		virtual bool isConnected() const = 0;

		virtual sf::TcpSocket &getSocket() = 0;
		virtual std::queue<sf::Packet>& getPacketsToSend() = 0;
		virtual std::queue<sf::Packet>& getReceivedPackets() = 0;

		virtual bool sendPacket(sf::Packet &) = 0;

		virtual void handleReceivedPackets() = 0;
		virtual void handleToSendPackets() = 0;

		virtual sf::Packet handleSendingPacket(sf::Packet) = 0;
		virtual sf::Packet handleReceivingPacket(sf::Packet) = 0;

		virtual void handlePackets() = 0;
		virtual void receivePackets() = 0;
		virtual void listenConsole() = 0;

		virtual bool run() = 0;
		virtual void waitUntilExit() const = 0;
	};
}


