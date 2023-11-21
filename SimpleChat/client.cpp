#include "client.hpp"

#include <iostream>
#include <thread>
#include <string>

#include "logger.hpp"

namespace dsc {
	bool client::connect(const char *address, unsigned short port) {
		this->connectionState = socket.connect(address, port) == sf::Socket::Done;

		if (this->isConnected()) {
			logger::info("Connected to the server", logger::CLIENT);
		}
		else {
			logger::error("Could not connect to the server", logger::CLIENT);
		}

		return this->isConnected();
	}
	void client::disconnect() {
		connectionState = false;
		socket.disconnect();
		logger::info("Disconnected from the server", logger::CLIENT);
	}

	bool client::isConnected() const {
		return this->connectionState;
	}

	void client::receivePackets(sf::TcpSocket *socket) {
		while (this->isConnected()) {
			if (socket->receive(lastPacket) != sf::Socket::Done) {
				logger::error("Failed to receive packet", logger::CLIENT);
				logger::warning("Socket might not be connected", logger::CLIENT);
			}
			else {
				logger::msg("Packet received", logger::CLIENT);
				packets.push(lastPacket);
			}
		}

	}
	bool client::sendPacket(sf::Packet &packet) {
		if (packet.getDataSize() <= 0) {
			logger::warning("Empty packet", logger::CLIENT);
			return false;
		}

		switch (socket.send(packet)) {
			case sf::Socket::Done:
				logger::info("Packet sent", logger::CLIENT);
				return true;

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
		return false;
	}

	bool client::hasPackets() const {
		return packets.size() > 0;
	}
	sf::Packet client::getLatestPacket() const {
		return this->lastPacket;
	}
	sf::Packet client::nextPacket() {
		if (packets.size() > 0) {
			sf::Packet packet = packets.front();
			packets.pop();
			return packet;
		}
		return sf::Packet();
	}

	std::string client::getIpAddress() const {
		return sf::IpAddress::getLocalAddress().toString();
	}
	unsigned short client::getPort() const {
		return socket.getLocalPort();
	}

	void client::run() {
		std::thread receptionThread(&client::receivePackets, this, &socket);
		receptionThread.detach();
	}
}

