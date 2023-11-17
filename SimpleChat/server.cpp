#include "server.hpp"

#include <iostream>
#include <thread>

#include "logger.hpp"


namespace dsc {
    server::server(unsigned short port)
        : listenPort(port) {
        #ifdef DEBUG
        dsc::logger::setLogLevel(logger::DEBUG);
        dsc::logger::setRecordTrace(true);
        dsc::logger::setTraceLevel(logger::INFO);
        #else
        dsc::logger::setLogLevel(logger::WARNING);
        dsc::logger::setRecordTrace(false);
        #endif // DEBUG

        logger::info("Starting server...", logger::SERVER);

        if (listener.listen(listenPort) != sf::Socket::Done) {
            logger::error("Failed to listen to port", logger::SERVER);
            exit(1);
        }

        logger::info("Server started successfuly", logger::SERVER);
        logger::info("Address: " + this->getIpAddress(), logger::SERVER);
        logger::info(std::string("Port: ") + std::to_string(this->getPort()), logger::SERVER);
    }

    std::string server::getIpAddress() {
        return sf::IpAddress::getLocalAddress().toString();
    }
    unsigned short server::getPort() {
        return listener.getLocalPort();
    }

    void server::connectClients(std::vector<sf::TcpSocket *> *clientArray) {
        while (true) {
            sf::TcpSocket *newClient = new sf::TcpSocket();
            if (listener.accept(*newClient) == sf::Socket::Done) {
                newClient->setBlocking(false);
                clientArray->push_back(newClient);

                logger::buildMessage()
					.add("Added client ")
					.add(newClient->getRemoteAddress().toString())
					.add(":")
					.add(std::to_string(newClient->getRemotePort()))
					.add(" on slot ")
					.add(std::to_string(clientArray->size()))
					.setSender(logger::SERVER)
					.setSeverity(logger::INFO)
					.log();
            }
            else {
                logger::error("Server listener error, restart the server", logger::SERVER);
                delete (newClient);
                break;
            }
        }
    }
    void server::disconnectClient(sf::TcpSocket *socketPointer, size_t position) {
        logger::buildMessage()
            .add("Client ")
            .add(socketPointer->getRemoteAddress().toString())
            .add(":")
            .add(std::to_string(socketPointer->getRemotePort()))
            .add(" disconnected, removing")
            .setSender(logger::SERVER)
            .setSeverity(logger::INFO)
            .log();

        socketPointer->disconnect();
        delete (socketPointer);
        clientArray.erase(clientArray.begin() + position);
    }
    void server::broadcastPacket(sf::Packet &packet, sf::IpAddress excludeAddress, unsigned short port) {
        for (size_t iterator = 0; iterator < clientArray.size(); iterator++) {
            sf::TcpSocket *client = clientArray[iterator];

            if (client->getRemoteAddress() != excludeAddress || client->getRemotePort() != port) {

                if (client->send(packet) != sf::Socket::Done) {
                    logger::warning("Could not send packet on broadcast", logger::SERVER);
                    logger::buildMessage()
                        .add("Fail client: ")
                        .add(client->getRemoteAddress().toString())
                        .setSender(logger::SERVER)
                        .setSeverity(logger::INFO)
                        .log();
                }
            }
        }
    }
    void server::broadcastPacketBack(sf::Packet &packet, sf::TcpSocket *client) {
        if (client->send(packet) != sf::Socket::Done) {
            logger::warning("Could not send packet on broadcast", logger::SERVER);
            logger::buildMessage()
				.add("Fail client: ")
				.add(client->getRemoteAddress().toString())
				.setSender(logger::SERVER)
				.setSeverity(logger::INFO)
				.log();
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

            logger::buildMessage()
                .add("Received message from ")
                .add(client->getRemoteAddress().toString())
                .add(":")
                .add(std::to_string(client->getRemotePort()))
                .add(" '")
                .add(receivedMessage)
                .add("'")
                .setSender(logger::SERVER)
                .setSeverity(logger::VERBOSE)
                .log();

        }
    }
    void server::managePackets() {
        for (size_t iterator = 0; iterator < clientArray.size(); iterator++) {
            receivePacket(clientArray[iterator], iterator);
        }
    }
    void server::run() {
        std::thread connetionThread(&server::connectClients, this, &clientArray);
        while (true) {
            managePackets();

            std::this_thread::sleep_for((std::chrono::milliseconds)250);
        }
    }
}