#include "client.hpp"

#include <iostream>
#include <thread>
#include <string>

#define SERVER_LOG_LN(x) std::cout << x << std::endl
#define SERVER_LOG(x) std::cout << x

void client::connect(const char *address, unsigned short port) {
	this->connectionState = socket.connect(address, port) == sf::Socket::Done;

	if (this->connectionState) {
		SERVER_LOG_LN("[INFO/CLIENT]: Connected to the server");
	}	
	else {
		SERVER_LOG_LN("[ERROR/CLIENT]: Could not connect to the server");
	}
}
void client::disconnect() {
	socket.disconnect();
	connectionState = false;
	SERVER_LOG_LN("Disconnected from the server\n");
}

bool client::isConnected() {
	return this->connectionState;
}

void client::receivePacket(sf::TcpSocket *socket) {
	while (connectionState) {
		if (socket->receive(lastPacket) != sf::Socket::Done) {
			SERVER_LOG_LN("[ERROR/CLIENT]: Failed to receive packet");
			SERVER_LOG_LN("[WARN/CLIENT]: Socket might not be connected");
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
			SERVER_LOG_LN("[ERROR/CLIENT]: Socket disconnected");
			break;
		case sf::Socket::Error:
			SERVER_LOG_LN("[ERROR/CLIENT]: Socket error");
			break;
		case sf::Socket::Partial:
			SERVER_LOG_LN("[ERROR/CLIENT]: Partial packet sent");
			this->sendPacket(packet);
			break;
		default:
			SERVER_LOG_LN("[ERROR/CLIENT]: Unknown error");
	}
}
void client::getLatestPacket(sf::Packet &packet) {
	packet = this->lastPacket;
}

void client::run() {
	std::thread receptionThread(&client::receivePacket, this, &socket);
}