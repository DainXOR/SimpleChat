#include "server.hpp"

#include <iostream>
#include <thread>
#include <regex>
#include <chrono>

#include "logger.hpp"
#include "secrets.hpp"


namespace dsc {
    server::server(sf::Uint16 port)
        : listenPort(port), running(true), 
        onReceivePacket([](sf::Packet, sf::TcpSocket *) { return true; }) {

        this->modulus = secrets::generatePrimeNumber(1ull << 31ull);
        this->generator = secrets::generatePrimeNumber(1ull << 16ull);

        this->privateKey = secrets::generatePrimeNumber(1ull << 31ull);


        std::thread clientThread(&server::getNewClients, this);
        std::thread packetThread(&server::receivePackets, this);
        std::thread consoleThread(&server::listenConsole, this);
        std::thread requestThread(&server::manageRequests, this);

        clientThread.detach();
        packetThread.detach();
        consoleThread.detach();
        requestThread.detach();
    }

    std::string server::getIpAddress() const {
        return sf::IpAddress::getLocalAddress().toString();
    }
    sf::Uint16 server::getPort() const {
        return listener.getLocalPort();
    }
    bool server::isRunning() const {
		return running;
	}

    void server::getNewClients() {
        while (this->isRunning()) {
            if (listener.getLocalPort() == 0) {
				sf::sleep(sf::milliseconds(250));
				continue;
			}

            sf::TcpSocket *newClient = new sf::TcpSocket();
            bool isBanned = false;

            if (listener.accept(*newClient) != sf::Socket::Done) {
                logger::error("Failed to accept client", logger::SERVER);
				delete (newClient);
				continue;
			}

            newClient->setBlocking(false);
            requests.push({ newClient, CONNECT, sf::Packet() });

            sf::sleep(sf::milliseconds(250));
        }
    }
    void server::disconnectClient(sf::TcpSocket *socketPointer, int32_t pos) {
        logger::buildMessage()
            .add("Client ")
            .add(socketPointer->getRemoteAddress().toString())
            .add(":")
            .add(std::to_string(socketPointer->getRemotePort()))
            .add(" disconnected, removing")
            .setSender(logger::SERVER)
            .setSeverity(logger::DEBUG)
            .log();

        if (pos < 0) {
            pos = 0;
            int32_t size = static_cast<int32_t>(clients.size());
            for (; pos < size; pos++) {
                if (this->clients[pos].first == socketPointer) {
                    break;
                }
            }
        }

        socketPointer->disconnect();
        delete (socketPointer);
        clients.erase(clients.begin() + pos);
    }
    
    void server::broadcastPacket(sf::Packet &packet, sf::IpAddress excludeAddress, sf::Uint16 port) {
        for (auto &client : clients) {
            if (client.first->getRemoteAddress() != excludeAddress || client.first->getRemotePort() != port) {
                this->broadcastPacketBack(packet, client.first);
            }
        }
    }
    bool server::broadcastPacketBack(sf::Packet &packet, sf::TcpSocket *client, uint8_t attemps) {
        // Hard limit if packet is partialy sent 10 times
        sf::Socket::Status sendStatus = attemps > 10 ? 
            sf::Socket::Status::Error : client->send(packet);

        switch (sendStatus) {
            case sf::Socket::Done:
            {
                logger::msg("Packet sent on broadcast", logger::SERVER);
                return true;
            }
            case sf::Socket::Partial:
            {
                logger::warning("Partial packet sent on broadcast", logger::SERVER);
                logger::buildMessage()
                    .add("Fail client: ")
                    .add(client->getRemoteAddress().toString())
                    .add(":")
                    .add(std::to_string(client->getRemotePort()))
                    .add(" Status Code: ")
                    .add(std::to_string(sendStatus))
                    .setSender(logger::SERVER)
                    .setSeverity(logger::DEBUG)
                    .log();
                logger::debug("Resending packet...", logger::SERVER);
                return broadcastPacketBack(packet, client, attemps + 1);
                break;
            }
            default:
            {
                logger::error("Could not send packet on broadcast", logger::SERVER);
                logger::buildMessage()
                    .add("Fail client: ")
                    .add(client->getRemoteAddress().toString())
                    .add(":")
                    .add(std::to_string(client->getRemotePort()))
                    .add(" Status Code: ")
                    .add(std::to_string(sendStatus))
                    .setSender(logger::SERVER)
                    .setSeverity(logger::DEBUG)
                    .log();

                return false;
            }
        }

        if (sendStatus != sf::Socket::Done) {
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
        for (size_t i = 0; i < clients.size(); i++) {
            sendDisconnectMessage(clients[i].first);
			disconnectClient(clients[i].first, i);
		}
    }
    void server::sendDisconnectMessage(sf::TcpSocket *client) {
        sf::Packet packet;
		packet << "Disconnnected from server";
		broadcastPacketBack(packet, client);
    }

    bool server::getPacket(sf::TcpSocket *client, size_t iterator) {
        sf::Packet packet;
        if (client->receive(packet) == sf::Socket::Disconnected) {
            this->requests.push({ client, DISCONNECT, sf::Packet() });
            return true;
        }
        if (packet.getDataSize() > 0) {
            std::string receivedMessage;
            packet >> receivedMessage;

            bool isCommand = receivedMessage[0] == '/';
            this->requests.push({ client, isCommand ? SEND_COMMAND : SEND_PACKET , sf::Packet() << receivedMessage});

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
    void server::receivePackets() {
        static constexpr uint16_t loopsToReport = 30 * 60;
        static constexpr sf::Int32 sleepTime = 1000 / 60;

        size_t iterator = 0;
        size_t receivedPackets = 0;
        auto startTime = std::chrono::steady_clock::now();
        

        while (this->isRunning()) {
            if (listener.getLocalPort() == 0 || clients.size() == 0) {
                sf::sleep(sf::milliseconds(250));
                continue;
            }

            for (size_t i = 0; i < clients.size(); i++) {
                receivedPackets += getPacket(clients[i].first, i);
            }
            iterator++;

            if (iterator >= loopsToReport) {
                auto endTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

                logger::buildMessage()
                    .add("Retrieved ")
                    .add(std::to_string(receivedPackets))
                    .add(" packets in ")
                    .add(std::to_string(float(elapsedTime) / 1000.f))
                    .add(" seconds")
                    .setSender(logger::SERVER)
                    .setSeverity(logger::VERBOSE)
                    .log();

				iterator = 0;
				receivedPackets = 0;
                startTime = std::chrono::steady_clock::now();
			}

			sf::sleep(sf::milliseconds(sleepTime));
        }
    }
    void server::manageRequests() {
        while (this->isRunning()) {
            if (listener.getLocalPort() == 0 || requests.empty()) {
				sf::sleep(sf::milliseconds(250));
				continue;
			}

            auto &[socket, request, packet] = requests.front();
            requests.pop();

            switch (request) {
                case CONNECT:
                {
                    if (isIpAddressBanned(socket->getRemoteAddress())) {
                        packet.clear();
                        packet << logger::buildMessage()
							.add("You are banned from this server")
							.setSender(logger::SERVER)
							.setSeverity(logger::USER_HIGH)
							.getMessage();

                        broadcastPacketBack(packet, socket);

                        logger::buildMessage()
                            .add("Banned client ")
                            .add(socket->getRemoteAddress().toString())
                            .add(":")
                            .add(std::to_string(socket->getRemotePort()))
                            .add(" tried to connect")
                            .setSender(logger::SERVER)
                            .setSeverity(logger::INFO)
                            .log();

                        socket->disconnect();
                        delete (socket);
                    }
                    else {


                        clients.push_back({ socket, 0 });

                        logger::buildMessage()
                            .add("Added client ")
                            .add(socket->getRemoteAddress().toString())
                            .add(":")
                            .add(std::to_string(socket->getRemotePort()))
                            .add(" on slot ")
                            .add(std::to_string(clients.size()))
                            .setSender(logger::SERVER)
                            .setSeverity(logger::DEBUG)
                            .log();
                    }
                    break;
                }
                case DISCONNECT:
                {
                    sendDisconnectMessage(socket);
                    disconnectClient(socket, clients.size());
                    break;
                }
                case SEND_PACKET:
                {
                    if (this->onReceivePacket(packet, socket)) {
                        std::string receivedMessage;
                        packet >> receivedMessage;
                        packet.clear();
                        packet  << receivedMessage 
                                << socket->getRemoteAddress().toString() 
                                << socket->getRemotePort();

                        broadcastPacket(packet, socket->getRemoteAddress(), socket->getRemotePort());
                    }
					break;
				}
                case SEND_COMMAND: {
                    std::string command;
                    packet >> command;

                    if (!isAdmin(socket)) {
						logger::buildMessage()
							.add("Client ")
							.add(socket->getRemoteAddress().toString())
							.add(":")
							.add(std::to_string(socket->getRemotePort()))
							.add(" tried to send a command ")
                            .add("'" + command + "'")
							.setSender(logger::SERVER)
							.setSeverity(logger::INFO)
							.log();

                        sf::Packet packet;
                        packet << logger::buildMessage()
                            .add("Only administrators can send commands")
                            .setSender(logger::SERVER)
                            .setSeverity(logger::USER_HIGH)
                            .getMessage();

						broadcastPacketBack(packet, socket);
						break;
                    }

                    this->sendCommand(command);

                    logger::buildMessage()
						.add("User ")
                        .add(socket->getRemoteAddress().toString())
                        .add(":")
                        .add(std::to_string(socket->getRemotePort()))
                        .add(" sent command ")
						.add("'" + command + "'")
						.setSender(logger::SERVER)
						.setSeverity(logger::DEBUG)
						.log();

                    std::string response = logger::buildMessage()
                        .add("Command ")
                        .add("'" + command + "'")
                        .add(" executed")
                        .setSender(logger::SERVER)
                        .setSeverity(logger::USER_MEDIUM)
                        .getMessage();

                    packet.clear();
                    packet << response;
                    broadcastPacketBack(packet, socket);
                    
                    break;
                }

                default:
                {
                    logger::error("Unknown request", logger::SERVER);
                    std::string response = logger::buildMessage()
                        .add("Unknown request")
                        .setSender(logger::SERVER)
                        .setSeverity(logger::ERROR)
                        .getMessage();

                    packet.clear();
                    packet << response;
                    broadcastPacketBack(packet, socket);
                    break;
                }
            }
        }
    }
    void server::listenConsole() {
        logger::debug("Server online", logger::SERVER);

        std::string userInput = "";
        std::regex availableCommands[] = {
            std::regex("^\/start( -p [0-9]{1,5})?$"),
            std::regex("^\/exit$"),
            std::regex("^\/stop$"),
            std::regex("^\/restart( -p [0-9]{1,5})?$"),
            std::regex("^\/clients$"),
            std::regex("^\/kick(( -a (([0-9]{1,3})\.{0,1})+)|( -p [0-9]{1,5})){2}$"),
            std::regex("^\/ban(( -a (([0-9]{1,3})\.{0,1})+)|( -p [0-9]{1,5})){2}$"),
            std::regex("^\/help$")
		};

        while (this->isRunning()) {
            if (commands.size() < 1) {
                std::getline(std::cin, userInput);
                sendCommand(userInput);
            }

            userInput = commands.front();
            commands.pop();

            if(userInput.length() < 1) continue;

            if (std::regex_match(userInput, availableCommands[0])) {
				logger::user_m("Starting server...", logger::SERVER);

                userInput.erase(0, 9);
                this->start(userInput.length() > 0 ? 
                            std::stoi(userInput) : this->listenPort);
			}
            else if (std::regex_match(userInput, availableCommands[1])) {
                logger::user_m("Exiting...", logger::SERVER);
                this->stop();
                this->running = false;
            }
            else if (std::regex_match(userInput, availableCommands[2])) {
                this->stop();
            }
            else if (std::regex_match(userInput, availableCommands[3])) {
                logger::user_m("Restarting server...", logger::SERVER);
                this->stop();
                userInput.erase(0, 12);
                this->start(userInput.length() > 0 ?
                            std::stoi(userInput) : this->listenPort);
            }
            else if (std::regex_match(userInput, availableCommands[4])) {
                logger::user_h("Connected clients:", logger::SERVER);
                for (size_t i = 0; i < clients.size(); i++) {
                    logger::buildMessage()
                        .add(std::to_string(i))
						.add(": ")
						.add(clients[i].first->getRemoteAddress().toString())
						.add(":")
						.add(std::to_string(clients[i].first->getRemotePort()))
						.setSender(logger::SERVER)
						.setSeverity(logger::USER_MEDIUM)
						.log();
                }
            }
            else if (std::regex_match(userInput, availableCommands[5])) {
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

                for (size_t i = 0; i < clients.size(); i++) {
                    if (clients[i].first->getRemoteAddress() == address && 
                        clients[i].first->getRemotePort() == port) {
                        disconnectClient(clients[i].first, i);
                        break;
                    }
                }
            }
            else if (std::regex_match(userInput, availableCommands[6])) {
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

                for (size_t i = 0; i < clients.size(); i++) {
                    if (clients[i].first->getRemoteAddress() == address && 
                        clients[i].first->getRemotePort() == port) {
                        banIpAddress(address);

                        std::string response = logger::buildMessage()
                            .add("Banned IP address ")
                            .add(address.toString())
							.setSender(logger::SERVER)
							.setSeverity(logger::USER_MEDIUM)
							.getMessage();

                        broadcastPacketBack(sf::Packet() << response, clients[i].first);

                        break;
                    }
                }
            }
            else if (std::regex_match(userInput, availableCommands[7])) {
                logger::user_h("Available commands:", logger::SERVER);
                logger::user_h("/start -p [port] - Starts the server on the specified port", logger::SERVER);
                logger::user_h("/stop - Stops the server", logger::SERVER);
                logger::user_h("/restart -p [port] - Restarts the server on the specified port", logger::SERVER);
                logger::user_h("/clients - Lists all connected clients", logger::SERVER);
                logger::user_h("/packets - Lists all managed packets", logger::SERVER);
                logger::user_h("/kick -a [address] -p [port] - Kicks the specified client", logger::SERVER);
                logger::user_h("/ban -a [address] -p [port] - Bans the specified client", logger::SERVER);
                logger::user_h("/help - Shows this message", logger::SERVER);
                }

            else {
				logger::user_h("Unknown command, type /help for a list of commands", logger::SERVER);
			}

            sf::sleep(sf::milliseconds(250));
		}
    }
    void server::sendCommand(std::string command) {
    	this->commands.push(command);
    }

    bool server::banIpAddress(sf::IpAddress ip) {
        if (isIpAddressBanned(ip)) {
			logger::buildMessage()
				.add("IP address ")
				.add(ip.toString())
				.add(" is already banned")
				.setSender(logger::SERVER)
				.setSeverity(logger::DEBUG)
				.log();
			return false;
		}
        else {
			this->bannedIpAddresses.push_back(ip);
            sf::TcpSocket *socket = nullptr;

            for (auto &[client, _] : clients) {
                if (client->getRemoteAddress() == ip) {
					socket = client;
					break;
				}
            }

            if (socket != nullptr) {
                std::string message = logger::buildMessage()
					.add("You are now banned from this server")
					.setSender(logger::SERVER)
					.setSeverity(logger::USER_HIGH)
					.getMessage();

				sf::Packet packet;
				packet << message;
				broadcastPacketBack(packet, socket);
                disconnectClient(socket);
            }

            logger::buildMessage()
                .add("Banned IP address ")
                .add(ip.toString())
                .setSender(logger::SERVER)
                .setSeverity(logger::USER_MEDIUM)
                .log();

			return true;
		}
    }
    bool server::unbanIpAddress(sf::IpAddress ip) {
        if (!isIpAddressBanned(ip)) {
            logger::buildMessage()
                .add("IP address ")
				.add(ip.toString())
				.add(" is not banned")
				.setSender(logger::SERVER)
				.setSeverity(logger::DEBUG)
				.log();
            return false;
        }

        for (size_t i = 0; i < bannedIpAddresses.size(); i++) {
            if (bannedIpAddresses[i] == ip) {
				bannedIpAddresses.erase(bannedIpAddresses.begin() + i);
				logger::buildMessage()
					.add("Unbanned IP address ")
					.add(ip.toString())
					.setSender(logger::SERVER)
					.setSeverity(logger::USER_MEDIUM)
					.log();
				return true;
			}
		}
    }
    bool server::isIpAddressBanned(sf::IpAddress ip) {
        bool result = false;
        for (auto &bannedIp : bannedIpAddresses) {
            result |= bannedIp == ip;
        }
        return result;
    }

    bool server::addAdmin(sf::TcpSocket * user) {
        if (isAdmin(user)) {
            logger::buildMessage()
                .add("Client ")
				.add(user->getRemoteAddress().toString())
				.add(":")
				.add(std::to_string(user->getRemotePort()))
				.add(" is already an admin")
				.setSender(logger::SERVER)
				.setSeverity(logger::USER_MEDIUM)
				.log();

            return false;
        }
        else {
			this->admins.push_back(user);
			logger::buildMessage()
				.add("Client ")
                .add(user->getRemoteAddress().toString())
                .add(":")
                .add(std::to_string(user->getRemotePort()))
                .add(" is now an admin")
                .setSender(logger::SERVER)
                .setSeverity(logger::USER_MEDIUM)
                .log();

            return true;
        }
    }
    bool server::removeAdmin(sf::TcpSocket * user) {
        if (!isAdmin(user)) {
			logger::buildMessage()
				.add("Client ")
				.add(user->getRemoteAddress().toString())
				.add(":")
				.add(std::to_string(user->getRemotePort()))
				.add(" is not an admin")
				.setSender(logger::SERVER)
				.setSeverity(logger::USER_MEDIUM)
				.log();

            return false;
		}
        else {
            for (size_t i = 0; i < admins.size(); i++) {
                if (admins[i] == user) {
					admins.erase(admins.begin() + i);
					logger::buildMessage()
						.add("Client ")
						.add(user->getRemoteAddress().toString())
						.add(":")
						.add(std::to_string(user->getRemotePort()))
						.add(" is no longer an admin")
						.setSender(logger::SERVER)
						.setSeverity(logger::USER_MEDIUM)
						.log();

					return true;
				}
			}
		}
    }
    bool server::isAdmin(sf::TcpSocket * user) {
        if (user->getRemoteAddress() == sf::IpAddress::getLocalAddress()) return true;

        bool result = false;
        for (auto &admin : admins) {
			result |= admin == user;
		}
		return result;
    }

    void server::setReceivePacketCallback(std::function<bool(sf::Packet, sf::TcpSocket *)> callback) {
		this->onReceivePacket = callback;
	}

    bool server::start(sf::Uint16 port) {
        this->listenPort = port;

        if (this->listenPort < 0 || this->listenPort > 65535) {
            logger::error("Invalid port", logger::SERVER);
            logger::debug("Port: " + std::to_string(listenPort), logger::SERVER);
            logger::user_m("Server failed to start", logger::SERVER);
            return false;
        }

        sf::Socket::Status listenStatus = listener.listen(this->listenPort);
        if (listenStatus == sf::Socket::Done) {
            this->running = true;

            logger::user_m("Server ready", logger::SERVER);
            logger::user_m("Address: " + this->getIpAddress(), logger::SERVER);
            logger::user_m("Port: " + std::to_string(this->getPort()), logger::SERVER);
            return true;
        }
        else {
            logger::buildMessage()
                .add("Failed to listen to port ")
                .add(std::to_string(listenPort))
                .add(" Status Code: ")
                .add(std::to_string(listenStatus))
                .setSender(logger::SERVER)
                .setSeverity(logger::ERROR)
                .log();
            logger::user_m("Server failed to start", logger::SERVER);
            return false;
        }
    }
    bool server::stop() {
        if (listener.getLocalPort() != 0) {
            logger::user_m("Stopping server...", logger::SERVER);
            disconnectAllClients();
            listener.close();
            logger::user_m("Server stopped", logger::SERVER);
            return true;
        }
        else {
            logger::user_m("Server is not running", logger::SERVER);
            return false;
        }
    }

    void server::run() {
        this->sendCommand("/start");

	}
    void server::exit() {
        this->sendCommand("/exit");
    }

    void server::waitUntilEnd() const {
        while (this->isRunning()) {
			sf::sleep(sf::milliseconds(250));
		}
    }

    server::~server() {
        this->exit();

        logger::debug("Server offline", logger::SERVER);
    }

}