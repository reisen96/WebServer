#include "Socket.h"

Socket::~Socket()
{
	close();
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

void Socket::close() 
{
	closesocket(windowsSocket);
	WSACleanup();
}
