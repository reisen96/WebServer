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
		// Select
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

		// Handle
		for (std::vector<Socket>::iterator::value_type& socket : serverSockets)
		{
			if (FD_ISSET(socket.getWindowsSocket(), &receiveSockets))
			{
				if (socket.listenState())
				{
					socket.addMessage(); // Not yet implemented
				}
				else if (socket.receiveState())
				{
					socket.getRequest(); // Not yet implemented
				}
			}
		}

		for (std::vector<Socket>::iterator::value_type& socket : serverSockets)
		{
			if (FD_ISSET(socket.getWindowsSocket(), &sendSockets))
			{
				if (socket.sendState())
				{
					sendResponse(socket);
				}
			}
		}

	}
}

void WebServer::sendResponse(Socket& socket)
{
	socket.getClientResponse().RESPONSE_MESSAGE = { "HTTP/1.1 " };

	if (socket.checkValidResponse()) // Not yet implemented
	{
		socket.generateValidResponse(); // Not yet implemented
	}
	else 
	{
		socket.generateInvalidResponse(); // Not yet implemented
	}

	socket.getClientRequest().resetRequest();
	
	int total_size_to_send = socket.getClientResponse().RESPONSE_MESSAGE.size();
	int bytes_sent = send(socket.getWindowsSocket(), socket.getClientResponse().RESPONSE_MESSAGE.data(), total_size_to_send, 0);

	if (SOCKET_ERROR == bytes_sent)
	{
		std::cout << "Server: Error at send(): " << WSAGetLastError() << std::endl;
		return;
	}

	socket.setReceive();
	if (socket.getClientRequest().HEADER_CONNECTION == "close")
	{
		socket.setInactive();
	}
}