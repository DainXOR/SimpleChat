#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Network.hpp>

#include "server.hpp"
#include "client.hpp"

int main() {
    #ifdef NDEBUG
    server serverNetwork(2525);
    serverNetwork.run();
    #else
    std::cout << "Enter the server address: ";
    std::string serverAddress;
    std::getline(std::cin, serverAddress);

    std::cout << "Enter the server port: ";
    std::string serverPort;
    std::getline(std::cin, serverPort);

    client clientNetwork;
    clientNetwork.connect(serverAddress.c_str(), std::stoi(serverPort));
    clientNetwork.run();
    #endif //  NDEBUG

    
	return 0;
}

