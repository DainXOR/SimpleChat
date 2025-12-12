#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>
#include <SFML/Network.hpp>

#define MAX_RAW_DATA 256 // Max bytes supported on Raw Data mode

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

class ServerNetwork {
    sf::TcpListener listener;
    std::vector<sf::TcpSocket *> client_array;

    unsigned short listen_port;

public:
    ServerNetwork(unsigned short);
    void ConnectClients(std::vector<sf::TcpSocket *> *);
    void DisconnectClient(sf::TcpSocket *, size_t);

    void ReceivePacket(sf::TcpSocket *, size_t);

    void BroadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);

    void ManagePackets();
    void Run();
};




int main() {

    ServerNetwork server(2000);
    server.Run();



    /*


    // dsc::client clientNetwork;
    // clientNetwork.waitUntilExit();


    dsc::chat chatServer;
    chatServer.run();
    chatServer.waitUntilEnd();
    */

    //uint64_t i = (1ull << 63) - 1;
    ////for (uint64_t i = 922337203685479727; i < (1ull << 63); i++) {
    //auto start = std::chrono::high_resolution_clock::now();
    //std::cout << "Big " << i << ". " << dsc::secrets::isPrimeBig(i) << "\n";
    //std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms\n";
    //
    //start = std::chrono::high_resolution_clock::now();
	//std::cout << "Normal " << i << ". " << dsc::secrets::isPrime(i) << "\n";
    //std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms\n";
	//}


    //uint64_t value1 = dsc::secrets::generatePrimeNumber();
    //std::cout << "Prime: " << value1 << "\n";
    //
    //uint64_t value2 = dsc::secrets::generatePrimeNumber(1ull << 63);
    //std::cout << "Prime: " << value2 << "\n";
    //
    //uint64_t value3 = dsc::secrets::generatePrimeNumber(2, 1ull << 63);
    //std::cout << "Prime: " << value3 << "\n";
    //
    //uint64_t value4 = dsc::secrets::generatePrimeNumber(1ull << 62, 1ull << 63);
    //std::cout << "Prime: " << value4 << "\n";

    #ifdef NDEBUG
    

    #else
    //dsc::user userClient;

    #endif // NDEBUG

    // Application::EnableVisualStyles();
    // Application::SetCompatibleTextRenderingDefault(false);
    // Application::Run(gcnew SimpleChat::Form1());
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


ServerNetwork::ServerNetwork(unsigned short port) : listen_port(port) {
    logl("Chat Server Started");

    if (listener.listen(listen_port) != sf::Socket::Done) {
        logl("Could not listen");
    }
}

void ServerNetwork::ConnectClients(std::vector<sf::TcpSocket *> *client_array) {
    while (true) {
        sf::TcpSocket *new_client = new sf::TcpSocket();
        if (listener.accept(*new_client) == sf::Socket::Done) {
            new_client->setBlocking(false);
            client_array->push_back(new_client);
            logl("Added client " << new_client->getRemoteAddress() << ":" << new_client->getRemotePort() << " on slot " << client_array->size());
        }
        else {
            logl("Server listener error, restart the server");
            delete (new_client);
            break;
        }
    }
}

void ServerNetwork::DisconnectClient(sf::TcpSocket *socket_pointer, size_t position) {
    logl("Client " << socket_pointer->getRemoteAddress() << ":" << socket_pointer->getRemotePort() << " disconnected, removing");
    socket_pointer->disconnect();
    delete (socket_pointer);
    client_array.erase(client_array.begin() + position);
}

void ServerNetwork::BroadcastPacket(sf::Packet &packet, sf::IpAddress exclude_address, unsigned short port) {
    for (size_t iterator = 0; iterator < client_array.size(); iterator++) {
        sf::TcpSocket *client = client_array[iterator];
        if (client->getRemoteAddress() != exclude_address || client->getRemotePort() != port) {
            if (client->send(packet) != sf::Socket::Done) {
                logl("Could not send packet on broadcast");
            }
        }
    }
}

void ServerNetwork::ReceivePacket(sf::TcpSocket *client, size_t iterator) {
    sf::Packet packet;
    if (client->receive(packet) == sf::Socket::Disconnected) {
        DisconnectClient(client, iterator);
        return;
    }

    if (packet.getDataSize() > 0) {
        std::string received_message;
        packet >> received_message;
        packet.clear();

        packet << received_message << client->getRemoteAddress().toString() << client->getRemotePort();

        BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
        logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " '" << received_message << "'");
    }
}

void ServerNetwork::ManagePackets() {
    while (true) {
        for (size_t iterator = 0; iterator < client_array.size(); iterator++) {
            ReceivePacket(client_array[iterator], iterator);
        }

        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}

void ServerNetwork::Run() {
    std::thread connetion_thread(&ServerNetwork::ConnectClients, this, &client_array);

    ManagePackets();
}




