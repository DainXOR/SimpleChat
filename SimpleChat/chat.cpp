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
	}

	void chat::run() {
		this->serverNetwork->run();
		sf::sleep(sf::milliseconds(250));
		logger::msg("Waiting for people to join...", logger::CLIENT);
		
	}

	void chat::waitUntilEnd() const {
		this->serverNetwork->waitUntilEnd();
	}
}
