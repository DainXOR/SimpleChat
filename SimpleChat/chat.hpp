#pragma once
#include "server.hpp"

#include <string>

namespace dsc {
	class user;

	class chat {
	public:
		server *serverNetwork;
		std::vector<user *> users;

		bool stablishingConnection = false;

		uint16_t keyBase = 0;
		uint64_t mod = 0;

		uint64_t privateKey = 0;
		uint64_t publicKey = 0;
		uint64_t commonKey = 0;

	public:
		chat();
		void run();
		void waitUntilExit() const;

		uint64_t generateKeyBase();
		uint64_t generateMod();


	};

}

