#include "user.hpp"

#include <iostream>
#include <string>
#include <thread>

#include "logger.hpp"

namespace dsc {
	user::user() {
		std::srand(static_cast<uint32_t>(std::time(nullptr)));

		int random = std::rand() % 10000;
		this->username = "Unnamed_" + std::to_string(random);
		this->clientNetwork = new client();
		this->generatePrivateKey();
		//chat *serverNetwork;
	}

	bool user::connect(const char* ip, sf::Uint16 port) {
		if (ip != nullptr && port != 0)
			this->clientNetwork->connect(ip, port);

		return this->clientNetwork->isConnected();

		/*
		std::cout << "Do you want to join a chat or create one? (join/create): ";
		std::string answer;
		std::getline(std::cin, answer);

		for (uint32_t i = 0; i < answer.length(); i++) {
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
		*/
	}
	void user::disconnect() {
		this->clientNetwork->disconnect();
	}

	void user::sendMessage(std::string msg) {
		sf::Packet packet;
		std::string message = this->username + ":" + msg;

		packet << secrets::encrypt(message, secrets::CAESAR, "25");
		this->clientNetwork->sendPacket(packet);
	}

	void user::handleReceivedMessages() {
		while (!this->clientNetwork->packets.empty()) {
			if(this->state == NegociationState::FAILED){
				std::cout << "Failed to establish secure connection\n";
				this->clientNetwork->packets.pop();
				continue;
			}



			sf::Packet packet = this->clientNetwork->packets.front();
			this->clientNetwork->packets.pop();

			if (this->state > NegociationState::NOT_STARTED) {
				this->establishSecureConnection(packet);
			}
			else {
				std::string message;
				packet >> message;
				std::cout << message << "\n";

			}
		}
	}

	void user::requestSecureConnection() {
		sf::Packet packet;
		packet << "secure";
		this->clientNetwork->sendPacket(packet);
		this->state = NegociationState::REQUEST;
	}

	void user::establishSecureConnection(sf::Packet packet) {
		std::string response;
		packet >> response;

		switch (this->state) {
			case NegociationState::REQUEST:
				if (response.find("allowed:") != std::string::npos) {
					this->state = NegociationState::NEGOTIATION;

					std::string keyBase = response.substr(8);
					uint16_t base = std::stoi(keyBase.substr(0, keyBase.find(":")));
					uint64_t mod = std::stoi(keyBase);
					this->generatePublicKey(base, mod);

					sf::Packet packet;
					response = "pk:" + std::to_string(this->publicKey);
					packet << response;
					this->clientNetwork->sendPacket(packet);

				}
				else {
					this->state = NegociationState::FAILED;
				}
				break;
			case NegociationState::NEGOTIATION:
				if (response.find("pk:") != std::string::npos) {
					this->state = NegociationState::ESTABLISHED;

					std::string keyBase = response.substr(4);
					uint64_t sharedKey = std::stoi(keyBase.substr(0, keyBase.find(":")));
					uint64_t mod = std::stoi(keyBase);
					uint64_t base = sharedKey;

					for (uint64_t i = 0; i < this->privateKey; i++) {
						sharedKey = (sharedKey * base) % mod;
					}

					this->commonKey = sharedKey;
					this->state = NegociationState::ESTABLISHED;
					dsc::logger::user_m("Secure connection established", logger::CLIENT);
					dsc::logger::debug("Common key: " + std::to_string(this->commonKey), logger::CLIENT);
				}
				else {
					this->state = NegociationState::FAILED;
				}
				break;
			default:
				break;
		}

	}

	std::string user::setUsername(std::string newUsername) {
		this->username = newUsername;
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
		std::thread receptionThread(&client::receivePackets, this, &clientNetwork->socket);

		while (true) {
			if (this->clientNetwork->isConnected()) {
				std::string userInput;
				std::getline(std::cin, userInput);

				if (userInput.length() < 1)
					continue;

				sf::Packet packet;
				packet	<< userInput 
						<< this->clientNetwork->socket.getRemoteAddress().toString() 
						<< this->clientNetwork->socket.getRemotePort();
				this->clientNetwork->sendPacket(packet);
			}

			std::this_thread::sleep_for((std::chrono::milliseconds)250);
		}
	}
	uint64_t user::generatePrivateKey() {
		this->privateKey = secrets::generatePrimeNumber(1ull << 31);
		return this->privateKey;
	}
	uint64_t user::generatePublicKey(uint16_t base, uint64_t mod) {
		this->publicKey = base;
		
		for (uint64_t i = 0; i < this->privateKey; i++) {
			this->publicKey = (this->publicKey * base) % mod;
		}

	}
}
