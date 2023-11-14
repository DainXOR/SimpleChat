#include "client.hpp"

#include <iostream>
#include <thread>

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

client::client() {
	logl("Chat Client Started");
}

void client::connect(const char *address, unsigned short port) {
	if (socket.connect(address, port) != sf::Socket::Done) {
		logl("Could not connect to the server\n");
	}
	else {
		isConnected = true;
		logl("Connected to the server\n");
	}
}

void client::receivePackets(sf::TcpSocket *socket) {
	while (true) {
		if (socket->receive(lastPacket) == sf::Socket::Done) {
			std::string receivedString;
			std::string senderAddress;
			unsigned short senderPort;
			lastPacket >> receivedString >> senderAddress >> senderPort;
			logl("From (" << senderAddress << ":" << senderPort << "): " << receivedString);
		}

		std::this_thread::sleep_for((std::chrono::milliseconds)250);
	}
}

void client::sendPacket(sf::Packet &packet) {
	if (packet.getDataSize() > 0 && socket.send(packet) != sf::Socket::Done) {
		logl("Could not send packet");
	}
}

void client::run() {
	std::thread receptionThread(&client::receivePackets, this, &socket);

	while (true) {
		if (isConnected) {
			std::string userInput;
			std::getline(std::cin, userInput);

			if (userInput.length() < 1)
				continue;

			sf::Packet packet;
			packet << userInput << socket.getRemoteAddress().toString() << socket.getRemotePort();
			sendPacket(packet);
		}

		std::this_thread::sleep_for((std::chrono::milliseconds)250);
	}
}