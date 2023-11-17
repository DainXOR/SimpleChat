#pragma once
#include "server.hpp"

class user;

class chat {
public:
	server *serverNetwork;
	std::vector<user *> users;

public:
	chat();
	void run();

};

