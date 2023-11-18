#include "client.hpp"

#include <iostream>
#include <thread>
#include <string>

#include "logger.hpp"

namespace dsc {
	void client::connect(const char *address, unsigned short port) {
		this->connectionState = socket.connect(address, port) == sf::Socket::Done;

		if (this->connectionState) {
			logger::info("Connected to the server", logger::CLIENT);
		}
		else {
			logger::error("Could not connect to the server", logger::CLIENT);
		}
	}
	void client::disconnect() {
		socket.disconnect();
		connectionState = false;
		logger::info("Disconnected from the server", logger::CLIENT);
	}

	bool client::isConnected() {
		return this->connectionState;
	}

	void client::receivePacket(sf::TcpSocket *socket) {
		while (connectionState) {
			if (socket->receive(lastPacket) != sf::Socket::Done) {
				logger::error("Failed to receive packet", logger::CLIENT);
				logger::warning("Socket might not be connected", logger::CLIENT);
			}
		}

	}
	void client::sendPacket(sf::Packet &packet) {
		if (packet.getDataSize() <= 0)
			return;

		switch (socket.send(packet)) {
			case sf::Socket::Done:
				break;
			case sf::Socket::Disconnected:
				logger::error("Socket disconnected", logger::CLIENT);
				break;
			case sf::Socket::Error:
				logger::error("Socket error", logger::CLIENT);
				break;
			case sf::Socket::Partial:
				logger::warning("Partial packet sent", logger::CLIENT);
				logger::info("Resending packet", logger::CLIENT);
				this->sendPacket(packet);
				break;
			default:
				logger::error("Unknown error", logger::CLIENT);
		}
	}
	void client::getLatestPacket(sf::Packet &packet) {
		packet = this->lastPacket;
	}

	void client::run() {
		std::thread receptionThread(&client::receivePacket, this, &socket);
	}
}

