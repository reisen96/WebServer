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
					newSocket.setReceive(true);
					serverSockets.push_back(newSocket);
				}
				else if (serverSockets[i].receiveState())
					receiveRequest(serverSockets[i]);
			}
		}

		for (int i = 0; i < serverSockets.size() && readySockets > 0; ++i)
		{
			if (FD_ISSET(serverSockets[i].getWindowsSocket(), &sendSockets))
			{
				--readySockets;
				if (serverSockets[i].sendState())
					sendResponse(serverSockets[i]);
			}
		}
	}
}

HttpMessage WebServer::generateHttpResponse(HttpMessage& httpRequest)
{
	HttpMessage ourResponse;
	switch (ourResponse.getHttpMethod())
	{
		case HttpMethod::GET:
		{
			ourResponse.generateResponseForGET();
			break;
		}
		case HttpMethod::POST:
		{
			ourResponse.generateResponseForPOST();
			break;
		}
		case HttpMethod::PUT:
		{
			ourResponse.generateResponseForPUT();
			break;
		}
		case HttpMethod::OPTIONS:
		{
			ourResponse.generateResponseForOPTIONS();
			break;
		}
		case HttpMethod::HEAD:
		{
			ourResponse.generateResponseForHEAD();
			break;
		}
		case HttpMethod::HTTP_DELETE:
		{
			ourResponse.generateResponseForDELETE();
			break;
		}
		case HttpMethod::TRACE:
		{
			ourResponse.generateResponseForTRACE();
			break;
		}
		default:
		{
			ourResponse.generateResponseForINVALID();
			break;
		}
	}

	return ourResponse;
	// Client httpResponse;
	// if httpRequest.method == GET bla bla bla...
	// check query bla bla bla, maybe print to console
	// return httpResponse;
}

void WebServer::receiveHttpRequest(Socket& socket, int requestSize)
{
	// NAOR:
	// 
	// Primor said that we are not being tested for invalid values. So assuming input is valid,
	// So we just received a message from void WebServer::receiveRequest(Socket& socket)
	// And we should just send it. So this function is pretty much redundant.


	


	// Client newHttpRequest(socket.getBuffer(),getBufferPosition()-requestSize,getBufferPosition()); --> Check if valid http here...
	// socket.setRequest(newHttpRequest);
	// memcpy(...)
	// socket -= requestSize;
}

void WebServer::receiveRequest(Socket& socket)
{
	int bytesReceived = recv(socket.getWindowsSocket(), &socket[socket.getBufferPosition()], sizeof(socket.getBuffer()) - socket.getBufferPosition(), 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		// Error...
		return;
	}
	if (!bytesReceived)
	{
		socket.close();
		socket.setInactive(); // Remove later all inactive sockets...
		return;
	}
	socket += bytesReceived;
	receiveHttpRequest(socket, bytesReceived);
	socket.setSend(true);
}

void WebServer::sendResponse(Socket& socket)
{
	int bytesSent;
	char sendBuffer[bufferSize];
	HttpMessage httpRequest = socket.getRequest(), httpResponse = generateHttpResponse(httpRequest);
	// httpResponse to buffer...
	bytesSent = send(socket.getWindowsSocket(), sendBuffer, 0/* httpResponse.size... */, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		// Error...
		return;
	}
	socket.setSend(false);
}
