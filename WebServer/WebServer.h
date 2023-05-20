#pragma once

#include "Socket.h"

class WebServer
{
private:

	const unsigned short serverPort = 27015u;
	const int serverBacklog = 5;
	std::vector<Socket> serverSockets;
	fd_set receiveSockets;
	fd_set sendSockets;
	
public:

	WebServer();
	~WebServer();

	void sendResponse(Socket& socket);

	void run();
};

