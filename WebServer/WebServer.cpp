#include "WebServer.h"

WebServer::WebServer()
{
	Socket listenSocket;
	listenSocket.initialize("", serverPort, SOCK_STREAM, IPPROTO_TCP);
	listenSocket.bindToPort();
	listenSocket.setListen(serverBacklog);
	listenSocket.setMode(false);
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
		try 
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
			if (readySockets == SOCKET_ERROR) {
				std::cout << std::string("Select error: ") + std::to_string(WSAGetLastError()) << std::endl;
				throw NetworkException(std::string("Select error: ") + std::to_string(WSAGetLastError()));
			}

			for (int i = 0; i < serverSockets.size() && readySockets > 0; ++i)
			{
				if (FD_ISSET(serverSockets[i].getWindowsSocket(), &receiveSockets))
				{
					--readySockets;
					if (serverSockets[i].listenState())
					{
						serverSockets.push_back(serverSockets[i].acceptConnection());
						Socket& newSocket = serverSockets.back();
						newSocket.setMode(false);
						newSocket.setReceive(true);
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
		catch (std::exception serverException) 
		{
			std::cout << serverException.what() << std::endl;
		}
	}
}

HttpMessage WebServer::generateHttpResponse(HttpMessage* httpRequest)
{
	HttpMessage response;
	switch (response.getHttpMethod())
	{
		case HttpMethod::GET:
		{
			response.generateResponseForGET();
			break;
		}
		case HttpMethod::POST:
		{
			response.generateResponseForPOST();
			break;
		}
		case HttpMethod::PUT:
		{
			response.generateResponseForPUT();
			break;
		}
		case HttpMethod::OPTIONS:
		{
			response.generateResponseForOPTIONS();
			break;
		}
		case HttpMethod::HEAD:
		{
			response.generateResponseForHEAD();
			break;
		}
		case HttpMethod::HTTP_DELETE:
		{
			response.generateResponseForDELETE();
			break;
		}
		case HttpMethod::TRACE:
		{
			response.generateResponseForTRACE();
			break;
		}
		default:
		{
			response.generateResponseForINVALID();
			break;
		}
	}

	return response;
}

void WebServer::receiveHttpRequest(Socket& socket, int requestSize)
{
	HttpMessage* request = HttpMessage::buildRequest(socket.getBuffer(), socket.getBufferPosition() - requestSize - 1);
	socket.addRequest(request);
	socket -= requestSize;
}

void WebServer::receiveRequest(Socket& socket)
{
	int bytesReceived = recv(socket.getWindowsSocket(), socket.getBufferAtPosition(), socket.getBufferAvailableSpace(), 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		socket.close();
		socket.setInactive(); // Remove later all inactive sockets...
		std::cout << "Receive error: " + std::to_string(WSAGetLastError()) << std::endl;
		return;
	}
	if (!bytesReceived)
	{
		socket.close();
		socket.setInactive(); // Remove later all inactive sockets...
		return;
	}
	socket += bytesReceived + 1;
	socket[socket.getBufferPosition() - 1] = '\0';
	receiveHttpRequest(socket, bytesReceived);
	socket.setSend(true);
}

void WebServer::sendResponse(Socket& socket)
{
	int bytesSent;
	char sendBuffer[bufferSize];
	HttpMessage* httpRequest = socket.getRequest(), httpResponse = generateHttpResponse(httpRequest);
	// httpResponse to buffer...
	bytesSent = send(socket.getWindowsSocket(), sendBuffer, 0/* httpResponse.size... */, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		// Error...
		return;
	}
	socket.setSend(false);
}
