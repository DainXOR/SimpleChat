#pragma once
#include "chat.hpp"
#include "client.hpp"
#include "secrets.hpp"

namespace dsc {
	class user {
		enum NegociationState {
			FAILED = -1,

			NOT_STARTED = 0,

			REQUEST,
			NEGOTIATION,
			ESTABLISHED
		};

		std::string username;
		client *clientNetwork;

		NegociationState state = NOT_STARTED;
		uint64_t privateKey = 0;
		uint64_t publicKey = 0;
		uint64_t commonKey = 0;

	public:
		user();
		bool connect(const char*, sf::Uint16);
		void disconnect();

		void sendMessage(std::string);
		void handleReceivedMessages();

		void requestSecureConnection();
		void establishSecureConnection(sf::Packet);

		std::string setUsername(std::string);
		std::string getUsername();

		std::string messageFormatter(std::string, std::string, sf::Uint16);
		std::string messageEncripter(std::string, size_t);

		std::string getIpAddress();
		unsigned short getPort();

		void run();

	private:
		uint64_t generatePrivateKey();
		uint64_t generatePublicKey(uint16_t, uint64_t);
	};

}

