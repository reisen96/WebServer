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

	Client generateHttpResponse(Client& httpRequest);
	void receiveHttpRequest(Socket& socket, int requestSize);
	void receiveRequest(Socket& socket);
	void sendResponse(Socket& socket);
	
public:

	WebServer();
	~WebServer();

	void run();
};

