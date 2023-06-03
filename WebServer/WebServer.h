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

	void prepareSockets();
	HttpMessage* generateHttpResponse(HttpMessage* httpRequest);
	void receiveHttpRequest(Socket& socket, int requestSize);
	void receiveRequest(Socket& socket);
	void sendResponse(Socket& socket);

	void generateResponseForGET(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForPOST(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForPUT(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForOPTIONS(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForHEAD(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForDELETE(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForTRACE(HttpMessage* httpRequest, HttpMessage* httpResponse);
	void generateResponseForINVALID(HttpMessage* httpRequest, HttpMessage* httpResponse);

public:

	WebServer();
	~WebServer();

	void run();
};

