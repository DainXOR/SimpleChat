#include "simple_client.hpp"

#include <iostream>
#include <thread>
#include <string>
#include <queue>

#include "logger.hpp"

namespace dsc {

	simple_client::simple_client() {}

	bool simple_client::connect(const char *address, unsigned short port) {
		this->connectionState = socket.connect(address, port) == sf::Socket::Done;

		if (this->connectionState) {
			this->socket.setBlocking(false);
			this->run();
			logger::info("Connected to the server", logger::CLIENT);
			return true;
		}
		else {
			logger::error("Could not connect to the server", logger::CLIENT);
			return false;
		}
	}
	bool simple_client::disconnect() {
		socket.disconnect();
		connectionState = false;
		logger::info("Disconnected from the server", logger::CLIENT);
		return true;
	}

	bool simple_client::isConnected() const {
		return this->connectionState;
	}

	sf::TcpSocket & simple_client::getSocket() {
		return this->socket;
	}
	std::queue<sf::Packet>& simple_client::getPacketsToSend() {
		return this->packetsToSend;
	}
	std::queue<sf::Packet>& simple_client::getReceivedPackets() {
		return this->receivedPackets;
	}

	bool simple_client::sendPacket(sf::Packet &packet) {
		if (packet.getDataSize() <= 0)
			return false;

		switch (socket.send(packet)) {
			case sf::Socket::Done:
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
	void simple_client::receivePackets() {
		sf::Packet lastPacket;

		if (this->socket.receive(lastPacket) != sf::Socket::Done) {
			logger::error("Failed to receive packet", logger::CLIENT);
			logger::warning("Socket might not be connected", logger::CLIENT);
		}
		else {
			receivedPackets.push(lastPacket);
			logger::info("Received packet", logger::CLIENT);
		}
	}

	void simple_client::handleReceivedPackets() {
		while (this->isConnected()) {
			if (this->receivedPackets.empty()) {
				continue;
			}

			sf::Packet packet = this->receivedPackets.front();
			this->receivedPackets.pop();

			this->handleReceivingPacket(packet);
		}
	}
	void simple_client::handleToSendPackets() {
		while (this->isConnected()) {
			if (this->packetsToSend.empty()) {
				std::cout << "No packets to send\n";
				sf::sleep(sf::milliseconds(250));
				continue;
			}

			sf::Packet packet = this->packetsToSend.front();
			this->packetsToSend.pop();

			std::string msg;
			packet >> msg;
			std::cout << "Sending: " + msg + "\n";

			packet << msg;

			this->handleSendingPacket(packet);
		}
	}

	sf::Packet simple_client::handleSendingPacket(sf::Packet packet) {
		if (this->sendPacket(packet)) {
			logger::info("Sent packet", logger::CLIENT);
		}
		else {
			logger::error("Failed to send packet", logger::CLIENT);
		}
		return packet;
	}
	sf::Packet simple_client::handleReceivingPacket(sf::Packet packet) {
		std::string message;
		packet >> message;

		std::cout << message << "\n";
		return packet;
	}

	void simple_client::handlePackets() {
		while (this->isConnected()) {
			std::cout << "Handling packets\n";

			this->receivePackets();
			this->handleReceivedPackets();
			this->handleToSendPackets();

			sf::sleep(sf::milliseconds(150));
		}
	}
	void simple_client::listenConsole() {
		std::string input;

		while (this->isConnected()) {
			std::getline(std::cin, input);

			std::cout << "Msg recived\n";
			std::cout << "MSG: " + input + "\n";

			sf::Packet packet;
			packet << input;
			this->packetsToSend.push(packet);

			sf::sleep(sf::milliseconds(16));
		}
	}

	bool simple_client::run() {
		std::thread consoleThread(&simple_client::listenConsole, this);
		std::thread handleThread(&simple_client::handlePackets, this);

		consoleThread.detach();
		handleThread.detach();

		return true;
	}
	void simple_client::waitUntilExit() const {
		while (this->isConnected()) {
			sf::sleep(sf::milliseconds(250));
		}
	}

	
}
