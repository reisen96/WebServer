#include "WebServer.h"

WebServer::WebServer()
{
	Socket listenSocket;
	listenSocket.initialize("", serverPort, SOCK_STREAM, IPPROTO_TCP);
	listenSocket.bindToPort();
	listenSocket.setMode(false);
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
			if (socket.sendState())
				FD_SET(socket.getWindowsSocket(), &sendSockets);
		}
		readySockets = select(0, &receiveSockets, &sendSockets, NULL, NULL);
		if (readySockets == SOCKET_ERROR)
			throw NetworkException(std::string("Select error: ") + std::to_string(WSAGetLastError()));

		for (int i = 0; i < serverSockets.size() && readySockets > 0; ++i)
		{
			if (FD_ISSET(serverSockets[i].getWindowsSocket(), &receiveSockets))
			{
				--readySockets;
				if (serverSockets[i].listenState()) 
				{
					Socket newSocket = serverSockets[i].acceptConnection();
					newSocket.setMode(false);
					serverSockets.push_back(newSocket);
				}
				else if (serverSockets[i].receiveState())
					; // Receive...
			}
		}

		for (int i = 0; i < serverSockets.size() && readySockets > 0; ++i)
		{
			if (FD_ISSET(serverSockets[i].getWindowsSocket(), &sendSockets))
			{
				--readySockets;
				if (serverSockets[i].sendState())
					; // Send...
			}
		}
	}
}

void WebServer::receiveRequest(Socket& socket)
{

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