#include "Socket.h"

Socket::Socket()
{
	socketReceiveState = SocketState::Inactive;
	socketSendState = SocketState::Inactive;
}

Socket::Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress, SocketState receiveState, SocketState sendState)
{
	this->windowsSocket = windowsSocket;
	this->socketAddress = socketAddress;
	this->socketReceiveState = receiveState;
	this->socketSendState = sendState;
}

Socket::~Socket()
{
	close();
}

Socket& Socket::operator=(const Socket& other)
{
	if (this == &other)
		return *this;
	this->windowsSocket = other.windowsSocket;
	this->socketAddress = other.socketAddress;
	this->socketReceiveState = other.socketReceiveState;
	this->socketSendState = other.socketSendState;
	return *this;
}

char& Socket::operator[](int index) 
{
	return socketBuffer[index];
}

Socket Socket::acceptConnection()
{
	sockaddr_in incomingSocketAddress;
	int incomingSocketAddressLength = sizeof(incomingSocketAddress);
	SOCKET incomingSocket = accept(windowsSocket, (struct sockaddr*)&incomingSocketAddress, &incomingSocketAddressLength);
	if (incomingSocket == INVALID_SOCKET)
		throw NetworkException(std::string("TCP connection accept error: ") + std::to_string(WSAGetLastError()));
	return Socket(incomingSocket, incomingSocketAddress, SocketState::Receive);
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

void Socket::setListen(int backlog)
{
	if (listen(windowsSocket, backlog) == SOCKET_ERROR)
		throw NetworkException(std::string("Socket listening error: ") + std::to_string(WSAGetLastError()));
	socketReceiveState = SocketState::Listen;
}

void Socket::setMode(bool blocking) 
{
	unsigned long mode = blocking ? 0u : 1u;
	if (ioctlsocket(windowsSocket, FIONBIO, &mode) != 0)
		throw NetworkException(std::string("Socket mode error: ") + std::to_string(WSAGetLastError()));
}

void Socket::close() 
{
	socketReceiveState = SocketState::Inactive;
	socketSendState = SocketState::Inactive;
	closesocket(windowsSocket);
}

/*
bool Socket::checkValidResponse()
{

}

void Socket::generateValidResponse()
{

}

void Socket::generateInvalidResponse()
{

}

void Socket::addMessage()
{

}

void Socket::getRequest()
{
	if (get_message_from_socket(curr_socket))
	{
		getRequestDetails();
		// sock_handler.get_request_details(curr_socket);
	}
	else
		std::cout << "socket:" << windowsSocket << " contained corrupted/blank message. Aborting." << std::endl;
}

char* Socket::getMessage()
{
	char* cbuffer = new char[BUFF_SIZE] {0};
	int cbuffer_capacity = BUFF_SIZE;
	bool socketEmpty = false;
	int bytesReceived = 0, total_bytesReceived = 0;

	for (int iter = 0; !socketEmpty && iter < MAX_SOCKETS; ++iter)
	{
		bytesReceived = recv(windowsSocket, cbuffer + total_bytesReceived, cbuffer_capacity, NULL);

		if (!iter && (!bytesReceived || bytesReceived == SOCKET_ERROR))
		{
			std::cout << "serv: Error at recv(): " << WSAGetLastError() << std::endl;
			closesocket(windowsSocket);
			cleanSocket();
			delete[] cbuffer;
			return NULL;
		}

		if (iter && bytesReceived == SOCKET_ERROR) socketEmpty = true;
		else total_bytesReceived += bytesReceived;

		if (total_bytesReceived == cbuffer_capacity)
		{
			cbuffer_capacity *= 2;
			char* tmp = new char[cbuffer_capacity] {0};
			memcpy(tmp, cbuffer, total_bytesReceived);
			delete[] cbuffer;
			cbuffer = tmp;
		}
	}

	cbuffer[total_bytesReceived] = '\0';
	clientRequest.MESSAGE.resize(total_bytesReceived);
	clientRequest.MESSAGE.shrink_to_fit();

	socketReceiveState = SocketState::Send;
	clientRequest.getRequest();
	clientRequest.getHeader();

	return cbuffer;
}

void Socket::cleanSocket()
{
	socketReceiveState = SocketState::Inactive;
	windowsSocket = 0;
	clientRequest.cleanRequest();
	clientResponse.cleanResponse();
}



*/