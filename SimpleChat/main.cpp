#pragma once
#include <iostream>
#include <string>
#include <regex>

#include "chat.hpp"
#include "user.hpp"
#include "secrets.hpp"
#include "logger.hpp"

int main() {
    dsc::logger::setLogLevel(dsc::logger::VERBOSE);

    #ifdef NDEBUG
    dsc::chat chatServer;
    chatServer.run();

    #else
    dsc::user userClient;

    #endif // NDEBUG

    // dsc::chat chatServer;
    // chatServer.run();

    //
    //std::cout << "Enter the server address: ";
    //std::string serverAddress;
    //std::getline(std::cin, serverAddress);
    //
    //std::cout << "Enter the server port: ";
    //std::string serverPort;
    //std::getline(std::cin, serverPort);
    //
    //std::cout << "Enter your username: ";
    //std::string username;
    //std::getline(std::cin, username);
    //
    //client clientNetwork;
    //clientNetwork.connect(serverAddress.c_str(), std::stoi(serverPort));
    //clientNetwork.run();
    //std::string message = "Hello World";
    //
    //std::cout << message <<"\n";
    //std::cout << (message = secrets::encrypt("Hello World", secrets::CAESAR, "25")) << "\n";
    //std::cout << secrets::decrypt(message, secrets::CAESAR, "25") << "\n";
    //
	return 0;
}

