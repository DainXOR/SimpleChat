#include "base_client_decorator.hpp"

namespace dsc {
	base_client_decorator::base_client_decorator(abstract_client *client) {
		this->decoratedClient = client;
	}
	base_client_decorator::~base_client_decorator() {
		delete this->decoratedClient;
	}

	bool base_client_decorator::connect(const char *ip, unsigned short port) {
		return this->decoratedClient->connect(ip, port);
	}
	bool base_client_decorator::disconnect() {
		return this->decoratedClient->disconnect();
	}

	bool base_client_decorator::isConnected() const {
		return this->decoratedClient->isConnected();
	}

	abstract_client * base_client_decorator::getClient() {
		return this->decoratedClient;
	}

	sf::TcpSocket & base_client_decorator::getSocket() {
		return this->decoratedClient->getSocket();
	}
	std::queue<sf::Packet>& base_client_decorator::getPacketsToSend() {
		return this->decoratedClient->getPacketsToSend();
	}
	std::queue<sf::Packet>& base_client_decorator::getReceivedPackets() {
		return this->decoratedClient->getReceivedPackets();
	}

	bool base_client_decorator::sendPacket(sf::Packet &packet) {
		return this->decoratedClient->sendPacket(packet);
	}

	void base_client_decorator::handleReceivedPackets() {
		this->decoratedClient->handleReceivedPackets();
	}
	void base_client_decorator::handleToSendPackets() {
		this->decoratedClient->handleToSendPackets();
	}

	sf::Packet base_client_decorator::handleSendingPacket(sf::Packet packet) {
		return this->decoratedClient->handleSendingPacket(packet);
	}

	sf::Packet base_client_decorator::handleReceivingPacket(sf::Packet packet) {
		return this->decoratedClient->handleReceivingPacket(packet);
	}

	void base_client_decorator::receivePackets() {
		this->decoratedClient->receivePackets();
	}
	void base_client_decorator::handlePackets() {
		this->decoratedClient->handlePackets();
	}
	void base_client_decorator::listenConsole() {
		this->decoratedClient->listenConsole();
	}

	bool base_client_decorator::run() {
		return this->decoratedClient->run();
	}

	void base_client_decorator::waitUntilExit() const {
		this->decoratedClient->waitUntilExit();
	}



}

