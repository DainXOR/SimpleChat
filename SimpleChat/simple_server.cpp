#include "simple_server.hpp"

#include <iostream>
#include <thread>

#include "logger.hpp"

namespace dsc {
	simple_server::simple_server(sf::Uint16 port) {
		this->connectionState = listener.listen(port) == sf::Socket::Done;
		this->listenPort = this->listener.getLocalPort();

		if (this->connectionState) {
			logger::info("Server is listening on port " + std::to_string(this->listenPort), logger::SERVER);
		}
		else {
			logger::error("Could not listen on port " + std::to_string(this->listenPort), logger::SERVER);
		}

	}

	std::string simple_server::getIpAddress() const {
		return sf::IpAddress::getLocalAddress().toString();
	}
	sf::Uint16 simple_server::getPort() const {
		return this->listenPort;
	}
	bool simple_server::isRunning() const {
		return this->connectionState;
	}

	void simple_server::connectNewClients() {
		while (this->isRunning()) {
			sf::TcpSocket *client = new sf::TcpSocket;
			if (listener.accept(*client) == sf::Socket::Done) {
				client->setBlocking(false);
				this->clients.push_back(client);
				logger::info("New client connected", logger::SERVER);
			}
			else {
				logger::error("Failed to accept new client", logger::SERVER);
			}

			sf::sleep(sf::milliseconds(100));
		}
	}
	void simple_server::disconnectClients() {
		for (auto client : this->clients) {
			client->disconnect();
			delete client;
		}
		this->clients.clear();
	}

	void simple_server::receivePackets() {
		sf::Packet lastPacket;

		while (this->isRunning()) {
			for (auto client : this->clients) {
				sf::Socket::Status result = client->receive(lastPacket);

				if (result == sf::Socket::Disconnected) {
					logger::info("Client disconnected", logger::SERVER);
					this->clients.erase(std::find(this->clients.begin(), this->clients.end(), client));
					delete client;
					continue;
				}

				if (result != sf::Socket::Done) {
					logger::error("Failed to receive packet", logger::SERVER);
					logger::warning("Socket might not be connected", logger::SERVER);
				}
				else {
					this->packets.push({ lastPacket, client });

					logger::info("Received packet", logger::SERVER);
				}
			}
		}
	}
	void simple_server::broadcastPackets() {
		while (this->isRunning()) {
			if (this->packets.empty() || this->clients.empty()) {
				continue;
			}

			full_packet packet = this->packets.front();
			this->packets.pop();

			auto copy = packet.first;
			std::string msg;
			copy >> msg;

			std::cout << "Broadcasting packet\n";
			std::cout << "Packet:\n";
			std::cout << "\t" << msg << "\n";

			for (auto client : this->clients) {
				if(client == packet.second)
					continue;

				sf::Socket::Status result = client->send(packet.first);

				if (result == sf::Socket::Disconnected) {
					logger::info("Client disconnected", logger::SERVER);
					this->clients.erase(std::find(this->clients.begin(), this->clients.end(), client));
					delete client;
					continue;
				}

				if (result != sf::Socket::Done) {
					logger::error("Failed to send packet", logger::SERVER);
					logger::warning("Socket might not be connected", logger::SERVER);
				}
				else {
					logger::info("Sent packet", logger::SERVER);
				}
			}
		}
	}

	bool simple_server::run() {
		if (!this->isRunning()) {
			logger::error("Server is not running", logger::SERVER);
			return false;
		}

		std::thread connectNewClientsThread(&simple_server::connectNewClients, this);
		std::thread receivePacketsThread(&simple_server::receivePackets, this);
		std::thread broadcastPacketsThread(&simple_server::broadcastPackets, this);

		connectNewClientsThread.detach();
		receivePacketsThread.detach();
		broadcastPacketsThread.detach();

		return true;
	}
	void simple_server::waitUntilExit() const {
		while (this->isRunning()) {
			sf::sleep(sf::milliseconds(250));
		}
	}
}

