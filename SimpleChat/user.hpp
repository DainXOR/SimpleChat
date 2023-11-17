#pragma once
#include "server.hpp"
#include "client.hpp"
#include "secrets.hpp"

class user {
	std::string username;
	client *clientNetwork;
	server *serverNetwork;

public:
	user();
	void init();
	void sendPacket(sf::Packet &);
	std::string getUsername();

	std::string messageFormatter(std::string, std::string, sf::Uint16);
	std::string messageEncripter(std::string, size_t);

	std::string getIpAddress();
	unsigned short getPort();

	void run();
};

