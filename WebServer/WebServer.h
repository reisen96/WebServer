#pragma once

#include "Socket.h"

class WebServer
{
private:

	const unsigned short serverPort = 8080u;
	const int serverBacklog = 5;
	std::vector<Socket> serverSockets;
	fd_set receiveSockets;
	fd_set sendSockets;

	HttpMessage* generateHttpResponse(HttpMessage* httpRequest);
	void receiveHttpRequest(Socket& socket, int requestSize);
	void receiveRequest(Socket& socket);
	void sendResponse(Socket& socket);
	
public:

	WebServer();
	~WebServer();

	void run();
};

