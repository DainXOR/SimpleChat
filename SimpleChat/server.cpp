#include "server.hpp"

#include <iostream>
#include <thread>


#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

server::server(unsigned short port) : listenPort(port) {
    logl("Chat Server Started");
    
    if (listener.listen(listenPort) != sf::Socket::Done) {
        logl("Could not listen");
        exit(1);
    }

    std::cout << "Address: " << sf::IpAddress::getLocalAddress().toString() << "\n";
    std::cout << "Port: " << listenPort << "\n\n";
}
void server::connectClients(std::vector<sf::TcpSocket *> *clientArray) {
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
void server::disconnectClient(sf::TcpSocket *socketPointer, size_t position) {
    logl("Client " << socketPointer->getRemoteAddress() << ":" << socketPointer->getRemotePort() << " disconnected, removing");
    socketPointer->disconnect();
    delete (socketPointer);
    clientArray.erase(clientArray.begin() + position);
}
void server::broadcastPacket(sf::Packet &packet, sf::IpAddress excludeAddress, unsigned short port) {
    for (size_t iterator = 0; iterator < clientArray.size(); iterator++) {
        sf::TcpSocket *client = clientArray[iterator];
        if (client->getRemoteAddress() != excludeAddress || client->getRemotePort() != port) {
            if (client->send(packet) != sf::Socket::Done) {
                logl("Could not send packet on broadcast");
            }
        }
    }
}
void server::receivePacket(sf::TcpSocket *client, size_t iterator) {
    sf::Packet packet;
    if (client->receive(packet) == sf::Socket::Disconnected) {
        disconnectClient(client, iterator);
        return;
    }

    if (packet.getDataSize() > 0) {
        std::string receivedMessage;
        packet >> receivedMessage;
        packet.clear();

        packet << receivedMessage << client->getRemoteAddress().toString() << client->getRemotePort();

        broadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
        logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " '" << receivedMessage << "'");
    }
}
void server::managePackets() {
    while (true) {
        for (size_t iterator = 0; iterator < clientArray.size(); iterator++) {
            receivePacket(clientArray[iterator], iterator);
        }

        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}
void server::run() {
    std::thread connetionThread(&server::connectClients, this, &clientArray);

    managePackets();
}