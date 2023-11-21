#pragma once
#include <iostream>
//#include <string>
//#include <regex>

#include "pch.h"

#include "Form1.h"

#include "chat.hpp"
#include "user.hpp"
#include "secrets.hpp"
#include "logger.hpp"

using namespace System::Windows::Forms;

int main() {
    srand(time(NULL));

    dsc::logger::setLogLevel(dsc::logger::VERBOSE);
    //dsc::chat chatServer;
    //chatServer.run();
    //
    //chatServer.waitUntilEnd();

    uint64_t value1 = dsc::secrets::generatePrimeNumber();
    std::cout << "Prime: " << value1 << "\n";

    uint64_t value2 = dsc::secrets::generatePrimeNumber(1ull << 31);
    std::cout << "Prime: " << value2 << "\n";

    uint64_t value3 = dsc::secrets::generatePrimeNumber(2, 1ull << 16);
    std::cout << "Prime: " << value3 << "\n";

    uint64_t value4 = dsc::secrets::generatePrimeNumber(2ull << 15, 2ull << 16);
    std::cout << "Prime: " << value4 << "\n";

    #ifdef NDEBUG
    

    #else
    //dsc::user userClient;

    #endif // NDEBUG

    //Application::EnableVisualStyles();
    //Application::SetCompatibleTextRenderingDefault(false);
    //Application::Run(gcnew CppCLRWinFormsProject::Form1());
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

