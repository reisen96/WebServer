#include "Socket.h"

Socket::Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress)
{
	this->windowsSocket = windowsSocket;
	this->socketAddress = socketAddress;
}

Socket::~Socket()
{
	close();
}

Socket Socket::acceptConnection()
{
	sockaddr_in incomingSocketAddress;
	int incomingSocketAddressLength = sizeof(incomingSocketAddress);
	SOCKET incomingSocket = accept(windowsSocket, (struct sockaddr*)&incomingSocketAddress, &incomingSocketAddressLength);
	if (incomingSocket == INVALID_SOCKET) 
	{
		throw NetworkException(std::string("Connection accept error: ") + std::to_string(WSAGetLastError()));
	}
	return Socket(incomingSocket, incomingSocketAddress);
}

void Socket::initialize(const std::string& ipAddress, unsigned short port, int type, int protocol)
{
	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(majorWinsockVersion, minorWinsockVersion), &wsaData))
		throw NetworkException(std::string("Error initializing Winsock"));
	windowsSocket = socket(AF_INET, type, protocol);
	if (windowsSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw NetworkException(std::string("Error initializing socket: ") + std::to_string(WSAGetLastError()));
	}
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = ipAddress.length() ? inet_addr(ipAddress.c_str()) : INADDR_ANY;
	socketAddress.sin_port = htons(port);
}

void Socket::bindToPort()
{
	if (bind(windowsSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR)
	{
		close();
		throw NetworkException(std::string("Error binding socket: ") + std::to_string(WSAGetLastError()));
	}
}

void Socket::listenState(int backlog)
{
	if (listen(windowsSocket, backlog) == SOCKET_ERROR) 
	{
		WSACleanup();
		throw NetworkException(std::string("Socket listening error: ") + std::to_string(WSAGetLastError()));
	}
}

void Socket::setMode(bool blocking) 
{
	unsigned long mode = blocking ? 0u : 1u;
	if (ioctlsocket(windowsSocket, FIONBIO, &mode) != 0)
	{
		throw NetworkException(std::string("Socket mode error: ") + std::to_string(WSAGetLastError()));
	}
}

void Socket::close() 
{
	closesocket(windowsSocket);
	WSACleanup();
}
