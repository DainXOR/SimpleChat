#include "server.hpp"

#include <iostream>
#include <thread>
#include <regex>

#include "logger.hpp"


namespace dsc {
    server::server(unsigned short port)
        : listenPort(port) {
        logger::msg("Starting server...", logger::SERVER);

        if (listener.listen(listenPort) != sf::Socket::Done) {
            logger::error("Failed to listen to port", logger::SERVER);
            logger::debug("Port: " + std::to_string(listenPort), logger::SERVER);
            logger::fatal("Server failed to start", logger::SERVER);
            exit(1);
        }

        logger::info("Server started successfuly", logger::SERVER);

        logger::user_m("Server ready", logger::SERVER);
        logger::user_m("Address: " + this->getIpAddress(), logger::SERVER);
        logger::user_m("Port: " + std::to_string(this->getPort()), logger::SERVER);
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
					.setSeverity(logger::DEBUG)
					.log();
            }
            else {
                logger::error("Server listener error, restart the server", logger::SERVER);
                logger::user_h("An error has ocurred, please restart the server", logger::SERVER);
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
            .setSeverity(logger::DEBUG)
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
                        .setSeverity(logger::DEBUG)
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
				.setSeverity(logger::DEBUG)
				.log();
        }
    }

    void server::disconnectAllClients() {
        for (size_t i = 0; i < clientArray.size(); i++) {
            sendDisconnectMessage(clientArray[i]);
			disconnectClient(clientArray[i], i);
		}
    }
    void server::sendDisconnectMessage(sf::TcpSocket *client) {
        sf::Packet packet;
		packet << "Server stopped";
		broadcastPacketBack(packet, client);
    }

    bool server::receivePacket(sf::TcpSocket *client, size_t iterator) {
        sf::Packet packet;
        if (client->receive(packet) == sf::Socket::Disconnected) {
            disconnectClient(client, iterator);
            return true;
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
            return true;
        }
        return false;
    }
    void server::managePackets() {
        size_t managedPackets = 0;

        for (size_t i = 0; i < clientArray.size(); i++) {
            managedPackets += receivePacket(clientArray[i], i);
        }

        logger::buildMessage()
			.add("Managed ")
			.add(std::to_string(managedPackets))
			.add(" packets")
			.setSender(logger::SERVER)
			.setSeverity(logger::DEBUG)
			.log();
    }
    void server::listenConsole() {
        std::string userInput = "";
        std::regex commands[] = {
            std::regex("/exit"),
            std::regex("/stop"),
            std::regex("^\/restart( -p [0-9]{1,5})?$"),
            std::regex("/clients"),
            std::regex("/packets"),
            std::regex("/help"),
            std::regex("^\/kick(( -a (([0-9]{1,3})\.{0,1})+)|( -p [0-9]{1,5})){2}$"),
            std::regex("^\/ban(( -a (([0-9]{1,3})\.{0,1})+)|( -p [0-9]{1,5})){2}$")
		};

        while (userInput != "/exit") {
			std::getline(std::cin, userInput);

            if (userInput == "/exit") {
                logger::user_h("Exiting...", logger::SERVER);
                logger::user_h("Stopping server...", logger::SERVER);
                disconnectAllClients();
                listener.close();
                logger::user_h("Server stopped", logger::SERVER);
            }
            else if (userInput == "/stop") {
				logger::user_h("Stopping server...", logger::SERVER);
                disconnectAllClients();
				listener.close();
				logger::user_h("Server stopped", logger::SERVER);
			}
            else if (std::regex_match(userInput, commands[2])) {
                logger::user_h("Restarting server...", logger::SERVER);
            	
                if (listener.getLocalPort() != 0) {
                    disconnectAllClients();
					listener.close();
                }

                userInput.erase(0, 12);
                this->listenPort = userInput.size() > 0 ?
                    std::stoi(userInput) : 0;

                if (listener.listen(listenPort) != sf::Socket::Done) {
                    logger::error("Failed to listen to port", logger::SERVER);
                    logger::debug("Port: " + std::to_string(listenPort), logger::SERVER);
                }
                else {
					logger::user_h("Server ready", logger::SERVER);
					logger::user_h("Address: " + this->getIpAddress(), logger::SERVER);
					logger::user_h("Port: " + std::to_string(this->getPort()), logger::SERVER);
				}
            }
            else if (userInput == "/clients") {
				logger::user_h("Connected clients:", logger::SERVER);
                for (size_t i = 0; i < clientArray.size(); i++) {
                    logger::user_h(
						std::to_string(i) + ": " +
						clientArray[i]->getRemoteAddress().toString() + ":" +
						std::to_string(clientArray[i]->getRemotePort()),
						logger::SERVER
					);
				}
			}
            else if (userInput == "/packets") {
				logger::user_h("Managed packets: " + std::to_string(clientArray.size()), logger::SERVER);
			}
            else if (userInput == "/help") {
				logger::user_h("Available commands:", logger::SERVER);
				logger::user_h("/stop - Stops the server", logger::SERVER);
				logger::user_h("/clients - Lists connected clients", logger::SERVER);
				logger::user_h("/packets - Lists managed packets", logger::SERVER);
				logger::user_h("/help - Shows this message", logger::SERVER);
			}
            else if (std::regex_match(userInput, commands[6])) {
                userInput.replace(0, 6, ""); 

                int aFlag = userInput.find("-a");
                int pFlag = userInput.find("-p");
                int max = aFlag > pFlag ? aFlag : pFlag;

                std::string t1 = userInput.substr(0, max);
                t1.erase(0, t1.find(" ") + 1);
                t1.pop_back();

                std::string t2 = userInput.substr(max, userInput.length());
                t2.erase(0, t2.find(" ") + 1);

                {
                    bool isIP = t1.find('.') != t1.npos;
                    t1 = isIP ? t1 : t2;
                    t2 = isIP ? t2 : t1;
                }

                size_t p1 = t1.find('.');
                size_t p2 = t1.find('.', p1 + 1);
                size_t p3 = t1.find('.', p2 + 1);

                sf::Uint8 byte1 = std::stoi(t1.substr(0, p1));
                sf::Uint8 byte2 = std::stoi(t1.substr(p1 + 1, p2));
                sf::Uint8 byte3 = std::stoi(t1.substr(p2 + 1, p3));
                sf::Uint8 byte4 = std::stoi(t1.substr(p3 + 1, t1.length()));

                sf::IpAddress address = sf::IpAddress(byte1, byte2, byte3, byte4);
                unsigned short port = std::stoi(t2);

                for (size_t i = 0; i < clientArray.size(); i++) {
                    if (clientArray[i]->getRemoteAddress() == address && clientArray[i]->getRemotePort() == port) {
						disconnectClient(clientArray[i], i);
						break;
					}
				}
			}
            else if (std::regex_match(userInput, commands[7])) {
				userInput.replace(0, 4, "");

				int aFlag = userInput.find("-a");
				int pFlag = userInput.find("-p");
				int max = aFlag > pFlag ? aFlag : pFlag;

				std::string t1 = userInput.substr(0, max);
				t1.erase(0, t1.find(" ") + 1);
				t1.pop_back();

				std::string t2 = userInput.substr(max, userInput.length());
				t2.erase(0, t2.find(" ") + 1);

                {
					bool isIP = t1.find('.') != t1.npos;
					t1 = isIP ? t1 : t2;
					t2 = isIP ? t2 : t1;
				}

				size_t p1 = t1.find('.');
				size_t p2 = t1.find('.', p1 + 1);
				size_t p3 = t1.find('.', p2 + 1);

				sf::Uint8 byte1 = std::stoi(t1.substr(0, p1));
				sf::Uint8 byte2 = std::stoi(t1.substr(p1 + 1, p2));
				sf::Uint8 byte3 = std::stoi(t1.substr(p2 + 1, p3));
				sf::Uint8 byte4 = std::stoi(t1.substr(p3 + 1, t1.length()));

				sf::IpAddress address = sf::IpAddress(byte1, byte2, byte3, byte4);
				unsigned short port = std::stoi(t2);

                for (size_t i = 0; i < clientArray.size(); i++) {
					if (clientArray[i]->getRemoteAddress() == address && clientArray[i]->getRemotePort() == port) {
                        disconnectClient(clientArray[i], i);
                        // TODO: Ban client
                    }
                }
            }
            
            else {
				logger::user_h("Unknown command, type /help for a list of commands", logger::SERVER);
			}
		}
    }

    void server::run() {
        std::thread packetManagerThread(&server::connectClients, this, &clientArray);
        std::thread clientConnectionThread(&server::managePackets, this);
        this->listenConsole();
        
    }
}