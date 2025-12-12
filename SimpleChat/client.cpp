#include "client.hpp"

#include <iostream>
#include <thread>
#include <string>

#include "secrets.hpp"
#include "logger.hpp"

namespace dsc {

	client::client() {
		this->privateKey = secrets::generatePrimeNumber();
		std::cout << "Address: ";
		std::string address;
		std::getline(std::cin, address);

		std::cout << "Port: ";
		std::string port;
		std::getline(std::cin, port);

		this->connect(address.c_str(), std::stoi(port));
	}

	bool client::connect(const char *address, unsigned short port) {
		this->connectionState = socket.connect(address, port) == sf::Socket::Done;

		if (this->connectionState) {
			this->run();
			logger::info("Connected to the server", logger::CLIENT);
			return true;
		}
		else {
			logger::error("Could not connect to the server", logger::CLIENT);
			return false;
		}
	}
	bool client::disconnect() {
		socket.disconnect();
		connectionState = false;
		logger::info("Disconnected from the server", logger::CLIENT);
		return true;
	}

	bool client::isConnected() const {
		return this->connectionState;
	}

	void client::receivePackets() {
		sf::Packet lastPacket;

		while (this->isConnected()) {
			if (this->socket.receive(lastPacket) != sf::Socket::Done) {
				logger::error("Failed to receive packet", logger::CLIENT);
				logger::warning("Socket might not be connected", logger::CLIENT);
			}
			else {
				packets.push(lastPacket);
				logger::info("Received packet", logger::CLIENT);
			}
		}
	}


	bool client::sendPacket(sf::Packet &packet) {
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

	void client::handlePackets() {
		while (this->isConnected()) {
			if (this->packets.size() < 1) {
				continue;
			}

			sf::Packet packet = this->packets.front();
			this->packets.pop();
			std::string msg;
			packet >> msg;

			if (msg.find("dh:") != msg.npos) {
				msg.erase(0, 3);
				uint64_t modPos = msg.find("|");
				uint64_t genPos = msg.find("|", modPos);
				uint64_t keyPos = msg.find("|", genPos);

				this->modulus = std::stoull(msg.substr(0, modPos));
				uint64_t generator = std::stoull(msg.substr(modPos + 1, genPos));
				uint64_t key = std::stoull(msg.substr(genPos + 1, keyPos));

				this->publicKey = secrets::modularExponentiation(generator, this->privateKey, this->modulus);

				packet.clear();
				packet << "dh:" << this->publicKey;
				this->sendPacket(packet);

				uint64_t sharedKey = secrets::modularExponentiation(key, this->privateKey, this->modulus);
				this->publicKey = sharedKey;
			} 
			else if (msg == "hs:request") {
				packet.clear();
				packet << "hs:" + std::to_string(this->publicKey);
				this->sendPacket(packet);
			}
			else if (msg == "hs:success") {
				logger::info("Handshake successful", logger::CLIENT);
			}
			else {
				std::string decryptedMsg = this->decrypt(msg, this->publicKey);
				std::cout << decryptedMsg << std::endl;
			}

		}
	}

	void client::listenConsole() {
		while (this->isConnected()) {
			std::string userInput;
			std::getline(std::cin, userInput);

			if (userInput.length() < 1)
				continue;

			sf::Packet packet;
			packet << this->encrypt(userInput, this->publicKey);
			this->sendPacket(packet);
		}
	}

	std::string client::encrypt(std::string msg, size_t key) {
		return dsc::secrets::encrypt(msg, secrets::CAESAR, std::to_string(key));
	}
	std::string client::decrypt(std::string msg, size_t key) {
		return dsc::secrets::decrypt(msg, secrets::CAESAR, std::to_string(key));
	}

	bool client::run() {
		std::thread receptionThread(&client::receivePackets, this);
		std::thread handleThread(&client::handlePackets, this);
		std::thread consoleThread(&client::listenConsole, this);

		receptionThread.detach();
		handleThread.detach();
		consoleThread.detach();
		return true;
	}
	void client::waitUntilExit() const {
		while (this->isConnected()) {
			sf::sleep(sf::milliseconds(250));
		}
	}
}

