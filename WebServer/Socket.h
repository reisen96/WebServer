#pragma once

#include "HttpMessage.h"

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
	int bufferPosition;
	char socketBuffer[bufferSize];
	HttpMessage httpRequest;

	Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress, SocketState receiveState = SocketState::Inactive, SocketState sendState = SocketState::Inactive);
	
public:

	Socket();
	~Socket();

	Socket& operator=(const Socket& other);

	SOCKET getWindowsSocket() { return windowsSocket; }
	char* getBuffer() { return socketBuffer; }
	int getBufferSize() { return bufferSize; }
	int getBufferPosition() { return bufferPosition; }
	
	char& operator[](int index);
	Socket& operator+=(const int difference);
	Socket& operator-=(const int difference);

	Socket acceptConnection();

	bool isInactive() { return socketReceiveState == SocketState::Inactive && socketSendState == SocketState::Inactive; }
	bool listenState() { return socketReceiveState == SocketState::Listen; }
	bool receiveState() { return socketReceiveState == SocketState::Receive; }
	bool sendState() { return socketSendState == SocketState::Send; }
	
	void initialize(const std::string& ipAddress, unsigned short port, int type = SOCK_DGRAM, int protocol = IPPROTO_UDP);

	void bindToPort();
	void setListen(int backlog = 0);
	void setMode(bool blocking);
	void setReceive(bool receiveState) { socketReceiveState = receiveState ? SocketState::Receive : SocketState::Inactive; }
	void setSend(bool sendState) { socketSendState = sendState ? SocketState::Send : SocketState::Inactive; }
	void setInactive() { socketReceiveState = socketSendState = SocketState::Inactive; }
	void close();

	void setRequest(HttpMessage& httpRequest) { this->httpRequest = httpRequest; }
	HttpMessage& getRequest() { return httpRequest; }


	// Client clientRequest, clientResponse; // ???
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
