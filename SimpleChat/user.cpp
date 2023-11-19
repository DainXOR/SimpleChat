#include "user.hpp"

#include <iostream>
#include <string>
#include <thread>

namespace dsc {
	user::user() {
		std::srand(std::time(nullptr));
		int random = std::rand() % 10000;
		this->username = "unnamed" + std::to_string(random);
		this->clientNetwork = new client();

		this->init();
	}

	void user::init() {
		std::cout << "Do you want to join a chat or create one? (join/create): ";
		std::string answer;
		std::getline(std::cin, answer);

		for (int i = 0; i < answer.length(); i++) {
			answer[i] = std::tolower(answer[i]);
		}

		if (answer == "join") {
			std::cout << "Enter the server address: ";
			std::string serverAddress;
			std::getline(std::cin, serverAddress);

			std::cout << "Enter the server port: ";
			std::string serverPort;
			std::getline(std::cin, serverPort);

			std::cout << "Enter your username: ";
			std::string username;
			std::getline(std::cin, username);

			this->clientNetwork->connect(serverAddress.c_str(), std::stoi(serverPort));
			this->clientNetwork->run();
		}
		else if (answer == "create") {
			this->serverNetwork = new server(sf::Socket::AnyPort);
			this->serverNetwork->sendCommand("/start");
			std::cout << "Waiting for people to join...\n";
		}
		else {
			std::cout << "Invalid answer\n";
			this->init();
		}
	}

	void user::sendPacket(sf::Packet &packet) {
		std::string msg;
		std::string username;
		sf::Uint16 port;
		packet >> msg >> username >> port;

		// packet >> 
		this->clientNetwork->sendPacket(packet);
	}

	std::string user::getUsername() {
		return this->username;
	}

	std::string user::messageFormatter(std::string msg, std::string ip, sf::Uint16 port) {
		return msg;
	}

	std::string user::messageEncripter(std::string, size_t) {
		return std::string();
	}

	std::string user::getIpAddress() {
		return sf::IpAddress::getLocalAddress().toString();
	}
	unsigned short user::getPort() {
		return clientNetwork->socket.getLocalPort();
	}

	void user::run() {
		while (true) {
			if (this->clientNetwork->isConnected()) {
				std::string userInput;
				std::getline(std::cin, userInput);

				if (userInput.length() < 1)
					continue;

				if (userInput == "/disconnect") {
					this->clientNetwork->disconnect();
					continue;
				}

				sf::Packet packet;
				packet << userInput
					<< this->clientNetwork->socket.getRemoteAddress().toString()
					<< this->clientNetwork->socket.getRemotePort();
				sendPacket(packet);
			}
			else {
				std::string userInput;
				std::getline(std::cin, userInput);

				if (userInput.length() < 1)
					continue;

				if (userInput == "/connect") {
					std::cout << "Enter the server address: ";
					std::string serverAddress;
					std::getline(std::cin, serverAddress);

					std::cout << "Enter the server port: ";
					std::string serverPort;
					std::getline(std::cin, serverPort);

					this->clientNetwork->connect(serverAddress.c_str(), std::stoi(serverPort));
					continue;
				}
			}

			std::this_thread::sleep_for((std::chrono::milliseconds)250);
		}
	}
}
