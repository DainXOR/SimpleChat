#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Network.hpp>

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

constexpr auto MAX_RAW_DATA = 256; // Max bytes supported on Raw Data mode;

class ServerNetwork {
    sf::TcpListener listener;
    std::vector<sf::TcpSocket *> clientArray;

    unsigned short listenPort;

public:
    ServerNetwork(unsigned short);
    void ConnectClients(std::vector<sf::TcpSocket *> *);
    void DisconnectClient(sf::TcpSocket *, size_t);

    void ReceivePacket(sf::TcpSocket *, size_t);

    void BroadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);

    void ManagePackets();
    void Run();
};

class ClientNetwork {
    sf::TcpSocket socket;
    sf::Packet lastPacket;

    bool isConnected = false;
public:
    ClientNetwork();
    void Connect(const char *, unsigned short);
    void ReceivePackets(sf::TcpSocket *);
    void SendPacket(sf::Packet &);
    void Run();
};

int main() {
    #ifdef NDEBUG
    ServerNetwork serverNetwork(2525);
    serverNetwork.Run();
    #else
    std::cout << "Enter the server address: ";
    std::string serverAddress;
    std::getline(std::cin, serverAddress);

    std::cout << "Enter the server port: ";
    std::string serverPort;
    std::getline(std::cin, serverPort);

    ClientNetwork client_network;
    client_network.Connect(serverAddress.c_str(), std::stoi(serverPort));
    client_network.Run();
    #endif //  NDEBUG

    
	return 0;
}

ServerNetwork::ServerNetwork(unsigned short port) : listenPort(port) {
    logl("Chat Server Started");
    
    if (listener.listen(listenPort) != sf::Socket::Done) {
        logl("Could not listen");
        exit(1);
    }

    std::cout << "Address: " << sf::IpAddress::getLocalAddress().toString() << "\n";
    std::cout << "Port: " << listenPort << "\n\n";
}
void ServerNetwork::ConnectClients(std::vector<sf::TcpSocket *> *clientArray) {
    while (true) {
        sf::TcpSocket *newClient = new sf::TcpSocket();
        if (listener.accept(*newClient) == sf::Socket::Done) {
            newClient->setBlocking(false);
            clientArray->push_back(newClient);
            logl("Added client " << newClient->getRemoteAddress() << ":" << newClient->getRemotePort() << " on slot " << clientArray->size());
        }
        else {
            logl("Server listener error, restart the server");
            delete (newClient);
            break;
        }
    }
}
void ServerNetwork::DisconnectClient(sf::TcpSocket *socketPointer, size_t position) {
    logl("Client " << socketPointer->getRemoteAddress() << ":" << socketPointer->getRemotePort() << " disconnected, removing");
    socketPointer->disconnect();
    delete (socketPointer);
    clientArray.erase(clientArray.begin() + position);
}
void ServerNetwork::BroadcastPacket(sf::Packet &packet, sf::IpAddress excludeAddress, unsigned short port) {
    for (size_t iterator = 0; iterator < clientArray.size(); iterator++) {
        sf::TcpSocket *client = clientArray[iterator];
        if (client->getRemoteAddress() != excludeAddress || client->getRemotePort() != port) {
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
        std::string receivedMessage;
        packet >> receivedMessage;
        packet.clear();

        packet << receivedMessage << client->getRemoteAddress().toString() << client->getRemotePort();

        BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
        logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " '" << receivedMessage << "'");
    }
}
void ServerNetwork::ManagePackets() {
    while (true) {
        for (size_t iterator = 0; iterator < clientArray.size(); iterator++) {
            ReceivePacket(clientArray[iterator], iterator);
        }

        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}
void ServerNetwork::Run() {
    std::thread connetionThread(&ServerNetwork::ConnectClients, this, &clientArray);

    ManagePackets();
}

ClientNetwork::ClientNetwork() {
    logl("Chat Client Started");
}
void ClientNetwork::Connect(const char *address, unsigned short port) {
    if (socket.connect(address, port) != sf::Socket::Done) {
        logl("Could not connect to the server\n");
    }
    else {
        isConnected = true;
        logl("Connected to the server\n");
    }
}
void ClientNetwork::ReceivePackets(sf::TcpSocket *socket) {
    while (true) {
        if (socket->receive(lastPacket) == sf::Socket::Done) {
            std::string receivedString;
            std::string senderAddress;
            unsigned short senderPort;
            lastPacket >> receivedString >> senderAddress >> senderPort;
            logl("From (" << senderAddress << ":" << senderPort << "): " << receivedString);
        }

        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}
void ClientNetwork::SendPacket(sf::Packet &packet) {
    if (packet.getDataSize() > 0 && socket.send(packet) != sf::Socket::Done) {
        logl("Could not send packet");
    }
}
void ClientNetwork::Run() {
    std::thread receptionThread(&ClientNetwork::ReceivePackets, this, &socket);

    while (true) {
        if (isConnected) {
            std::string userInput;
            std::getline(std::cin, userInput);

            if (userInput.length() < 1)
                continue;

            sf::Packet replyPacket;
            replyPacket << userInput;

            SendPacket(replyPacket);
        }
    }
}