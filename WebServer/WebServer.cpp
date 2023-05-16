#include "WebServer.h"

WebServer::WebServer() 
{
	Socket listenSocket;
	listenSocket.initialize("", serverPort, SOCK_STREAM, IPPROTO_TCP);
	listenSocket.bindToPort();
	listenSocket.setListen(serverBacklog);
	serverSockets.push_back(listenSocket);
}

WebServer::~WebServer()
{
	for (std::vector<Socket>::iterator::value_type& socket : serverSockets)
		socket.close();
	WSACleanup();
}

void WebServer::run()
{
	int readySockets;
	while (true) 
	{
		FD_ZERO(&receiveSockets);
		FD_ZERO(&sendSockets);
		for (std::vector<Socket>::iterator::value_type& socket : serverSockets)
		{
			if (socket.listenState() || socket.receiveState())
				FD_SET(socket.getWindowsSocket(), &receiveSockets);
			else if (socket.sendState())
				FD_SET(socket.getWindowsSocket(), &sendSockets);
		}
		readySockets = select(0, &receiveSockets, &sendSockets, NULL, NULL);
		if (readySockets == SOCKET_ERROR)
			throw NetworkException(std::string("Select error: ") + std::to_string(WSAGetLastError()));


	}
}
