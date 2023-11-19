#include "chat.hpp"

#include <iostream>
#include <string>
#include <functional>
#include <optional>

#include <SFML/Network.hpp>

#include "user.hpp"

namespace dsc {
	chat::chat() {
		this->serverNetwork = new server(sf::Socket::AnyPort);
	}

	void chat::run() {
		this->serverNetwork->sendCommand("/start");
		std::cout << "Waiting for people to join...\n";
	}
}
