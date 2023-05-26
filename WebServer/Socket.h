#pragma once

#include "NetworkException.h"

constexpr int bufferSize = 4096;

class Socket
{
private:

	enum class SocketState {
		Inactive,
		Listen,
		Receive,
		Send
	};

	const int majorWinsockVersion = 2;
	const int minorWinsockVersion = 2;
	SOCKET windowsSocket;
	sockaddr_in socketAddress;
	SocketState socketReceiveState;
	SocketState socketSendState;
	char socketBuffer[bufferSize];

	// Client clientRequest, clientResponse; // ???

	Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress, SocketState receiveState = SocketState::Inactive, SocketState sendState = SocketState::Inactive);
	
public:

	Socket();
	~Socket();

	Socket& operator=(const Socket& other);

	SOCKET getWindowsSocket() { return windowsSocket; }
	char* getBuffer() { return socketBuffer; }
	int getBufferSize() { return bufferSize; }

	char& operator[](int index);

	Socket acceptConnection();

	bool isInactive() { return socketReceiveState == SocketState::Inactive && socketSendState == SocketState::Inactive; }
	bool listenState() { return socketReceiveState == SocketState::Listen; }
	bool receiveState() { return socketReceiveState == SocketState::Receive; }
	bool sendState() { return socketSendState == SocketState::Send; }
	
	void initialize(const std::string& ipAddress, unsigned short port, int type = SOCK_DGRAM, int protocol = IPPROTO_UDP);

	void bindToPort();
	void setListen(int backlog = 0);
	void setMode(bool blocking);
	void setReceive() { socketReceiveState = SocketState::Receive; }
	void setSend() { socketSendState = SocketState::Send; }
	void setInactive() { socketReceiveState = socketSendState = SocketState::Inactive; }
	void close();

	// Client getClientRequest() { return clientRequest; }
	// Client getClientResponse() { return clientResponse; }

	// bool checkValidResponse();
	// void generateValidResponse();
	// void generateInvalidResponse();
	// void addMessage();
	// void getRequest();
	// char* getMessage();
	// void cleanSocket();
};
