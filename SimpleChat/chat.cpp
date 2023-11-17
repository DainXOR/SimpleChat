#include "chat.hpp"

#include <iostream>
#include <string>
#include <functional>
#include <optional>

#include <SFML/Network.hpp>

#include "user.hpp"

chat::chat() {
	this->serverNetwork = new server(sf::Socket::AnyPort);
	std::cout << "Waiting for people to join...\n";
}

void chat::run() {
	this->serverNetwork->run();
}