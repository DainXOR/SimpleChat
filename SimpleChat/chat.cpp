#include "chat.hpp"

#include <iostream>
#include <string>
#include <functional>
#include <optional>

#include <SFML/Network.hpp>

#include "user.hpp"
#include "logger.hpp"

namespace dsc {
	chat::chat() {
		this->serverNetwork = new server();
		/*
		this->serverNetwork->setRecivePacketCallback([this](sf::Packet packet, sf::TcpSocket *socket) -> bool {
			std::string message;
			packet >> message;

			if (message == "secure") {
				packet.clear();

				std::string response = "allowed:";
				response += std::to_string(this->keyBase);
				response += ":";
				response += std::to_string(this->mod);
				packet << response;

				this->stablishingConnection = true;
				this->serverNetwork->broadcastPacketBack(packet, socket);
			}
			else if (this->stablishingConnection) {

				if (message.find("pk:") != message.npos) {
					message.erase(0, 3);
					uint64_t publicKey = std::stoull(message.substr(0, message.find(":")));
					uint64_t mod = std::stoull(message.substr(message.find(":") + 1));

					logger::info("Public key: " + std::to_string(publicKey), logger::SERVER);
					logger::info("Mod: " + std::to_string(mod), logger::SERVER);

					this->commonKey = publicKey;
					for (uint64_t i = 0; i < this->privateKey; i++) {
						this->commonKey *= publicKey;
						this->commonKey %= mod;
					}

					packet.clear();
					std::string response = "success";
					packet << response;
					this->serverNetwork->broadcastPacketBack(packet, socket);

					this->stablishingConnection = false;
					return false;
				}
				else {
					packet.clear();
					std::string response = "packet not recognized";
					packet << response;

					this->serverNetwork->broadcastPacketBack(packet, socket);

					return false;
				}

				std::string response;
				packet >> response;


			}
			return !this->stablishingConnection;
		});
		*/

		this->keyBase = static_cast<uint16_t>(this->generateKeyBase());
		this->mod = this->generateMod();

		this->privateKey = secrets::generatePrimeNumber(1ull << 31);

		this->publicKey = keyBase;
		for (uint64_t i = 0; i < this->privateKey; i++) {
			this->publicKey *= keyBase;
			this->publicKey %= mod;
		}
	}

	void chat::run() {
		this->serverNetwork->run();
		std::cout << "Waiting for people to join...\n";
		
	}

	void chat::waitUntilExit() const {
		while (this->serverNetwork->isRunning());
	}
	uint64_t chat::generateKeyBase() {
		return dsc::secrets::generatePrimeNumber(500);
	}
	uint64_t chat::generateMod() {
		return dsc::secrets::generatePrimeNumber(1ull << 31);
	}
}
