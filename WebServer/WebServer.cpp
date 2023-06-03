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
	serverSockets.clear();
	WSACleanup();
}

void WebServer::prepareSockets()
{
	std::vector<Socket>::iterator currentSocket = serverSockets.begin();
	while (currentSocket != serverSockets.end())
		if (currentSocket->isInactive())
			currentSocket = serverSockets.erase(currentSocket);
		else
			++currentSocket;
	FD_ZERO(&receiveSockets);
	FD_ZERO(&sendSockets);
	for (std::vector<Socket>::iterator::value_type& socket : serverSockets)
	{
		if (socket.listenState() || socket.receiveState())
			FD_SET(socket.getWindowsSocket(), &receiveSockets);
		if (socket.sendState())
			FD_SET(socket.getWindowsSocket(), &sendSockets);
	}
}

HttpMessage* WebServer::generateHttpResponse(HttpMessage* httpRequest)
{
	HttpMessage* httpResponse = new HttpMessage;
	httpResponse->initializeResponseHeaders();
	switch (httpRequest->getHttpMethod())
	{
		case HttpMethod::GET:
		{
			generateResponseForGET(httpRequest, httpResponse);
			break;
		}
		case HttpMethod::POST:
		{
			generateResponseForPOST(httpRequest, httpResponse);
			break;
		}
		case HttpMethod::PUT:
		{
			generateResponseForPUT(httpRequest, httpResponse);
			break;
		}
		case HttpMethod::OPTIONS:
		{
			generateResponseForOPTIONS(httpRequest, httpResponse);
			break;
		}
		case HttpMethod::HEAD:
		{
			generateResponseForHEAD(httpRequest, httpResponse);
			break;
		}
		case HttpMethod::HTTP_DELETE:
		{
			generateResponseForDELETE(httpRequest, httpResponse);
			break;
		}
		case HttpMethod::TRACE:
		{
			generateResponseForTRACE(httpRequest, httpResponse);
			break;
		}
		default:
		{
			generateResponseForINVALID(httpRequest, httpResponse);
			break;
		}
	}
	return httpResponse;
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
		socket.setInactive();
		std::cout << "Receive error: " + std::to_string(WSAGetLastError()) << std::endl;
		return;
	}
	if (!bytesReceived)
	{
		if (!socket.sendState()) 
		{
			socket.close();
			socket.setInactive();
		}
		return;
	}
	socket += bytesReceived + 1;
	socket[socket.getBufferPosition() - 1] = '\0';
	receiveHttpRequest(socket, bytesReceived);
	socket.setSend(true);
}

void WebServer::sendResponse(Socket& socket)
{
	int bytesSent, messageSize;
	char sendBuffer[bufferSize];
	HttpMessage* httpRequest = socket.getRequest(), * httpResponse = generateHttpResponse(httpRequest);
	messageSize = httpResponse->writeResponseToBuffer(sendBuffer);
	bytesSent = send(socket.getWindowsSocket(), sendBuffer, messageSize, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		socket.close();
		socket.setInactive();
		std::cout << "Send error: " + std::to_string(WSAGetLastError()) << std::endl;
		delete httpRequest;
		delete httpResponse;
		return;
	}
	socket.setSend(!socket.isRequestsQueueEmpty());
	delete httpRequest;
	delete httpResponse;
}

void WebServer::generateResponseForGET(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	std::ifstream htmlFile;
	std::stringstream buffer;
	std::string requestedResource;
	std::string resourcePath = httpRequest->getRequestPath(), resourceLanguage = httpRequest->getQueryParameter("lang");
	httpResponse->setHttpMethod(HttpMethod::GET);
	if (serverResources.count(resourcePath) != 0)
	{
		if (resourceLanguage == "he")
			requestedResource = serverResources.at("indexhe.htm");
		else
			requestedResource = serverResources.at(resourcePath);
		htmlFile.open(requestedResource, std::ios::in);
		buffer << htmlFile.rdbuf();
		htmlFile.close();
		httpResponse->setContentLength((int)buffer.str().length() + 1);
		httpResponse->setResponseBody(buffer.str());
		httpResponse->setStatusCode(200);
		httpResponse->setResponseMessage("OK");
	}
	else 
	{
		httpResponse->setStatusCode(404);
		httpResponse->setResponseMessage("Not Found");
	}
}

void WebServer::generateResponseForPOST(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	std::string resourcePath = httpRequest->getRequestPath(), requestStrings = httpRequest->getHttpBody();
	httpResponse->setHttpMethod(HttpMethod::POST);
	if (resourcePath == "echo") 
	{
		std::cout << "Request strings:" << std::endl << requestStrings << std::endl;
		httpResponse->setResponseBody("POST Success");
		httpResponse->setContentLength(sizeof("POST Success"));
		httpResponse->setStatusCode(200);
		httpResponse->setResponseMessage("OK");
	}
	else 
	{
		httpResponse->setResponseBody("POST Fail");
		httpResponse->setContentLength(sizeof("POST Fail"));
		httpResponse->setStatusCode(404);
		httpResponse->setResponseMessage("Not Found");
	}
}

void WebServer::generateResponseForPUT(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	httpResponse->setHttpMethod(HttpMethod::PUT);
}

void WebServer::generateResponseForOPTIONS(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	httpResponse->setHttpMethod(HttpMethod::OPTIONS);
	httpResponse->addResponseHeader("Allow", "OPTIONS,GET,HEAD,POST,PUT,DELETE,TRACE");
	httpResponse->setStatusCode(200);
	httpResponse->setResponseMessage("OK");
}

void WebServer::generateResponseForHEAD(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	generateResponseForGET(httpRequest, httpResponse);
	httpResponse->setHttpMethod(HttpMethod::HEAD);
	httpResponse->setResponseBody("");
}

void WebServer::generateResponseForDELETE(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	httpResponse->setHttpMethod(HttpMethod::HTTP_DELETE);
}

void WebServer::generateResponseForTRACE(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	char buffer[bufferSize];
	int contentLength = httpRequest->writeRequestToBuffer(buffer);
	std::string requestString(buffer);
	httpResponse->setHttpMethod(HttpMethod::TRACE);
	httpResponse->setResponseBody(requestString);
	httpResponse->setContentLength(contentLength);
	httpResponse->setStatusCode(200);
	httpResponse->setResponseMessage("OK");
}

void WebServer::generateResponseForINVALID(HttpMessage* httpRequest, HttpMessage* httpResponse)
{
	std::string responseString = "Invalid request";
	httpResponse->setHttpMethod(HttpMethod::NONE);
	httpResponse->setStatusCode(400);
	httpResponse->setResponseMessage("Bad Request");
	httpResponse->setResponseBody(responseString);
	httpResponse->setContentLength((int)responseString.length());
}

void WebServer::run()
{
	int readySockets;
	while (true)
	{
		try
		{
			prepareSockets();
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
		catch (const std::exception serverException)
		{
			std::cout << serverException.what() << std::endl;
		}
	}
}
