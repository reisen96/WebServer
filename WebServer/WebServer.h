#pragma once

#include "Socket.h"

class WebServer
{
private:

	const unsigned short serverPort = 27015u;
	const int serverBacklog = 5;
	Socket serverSocket;

public:

	WebServer();


	void run();
};

