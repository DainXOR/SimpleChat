#include "chat.hpp"

#include <iostream>
#include <string>
#include <functional>
#include <optional>

#include <SFML/Network.hpp>

#include "user.hpp"

namespace dsc {
	chat::chat() {
		this->serverNetwork = new server();
	}

	void chat::run() {
		this->serverNetwork->run();
		sf::sleep(sf::milliseconds(1000));
		std::cout << "Waiting for people to join...\n";
		
	}
}
