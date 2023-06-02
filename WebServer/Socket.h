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
	std::queue<HttpMessage*> httpRequests;

	Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress, SocketState receiveState = SocketState::Inactive, SocketState sendState = SocketState::Inactive);
	
public:

	Socket();

	Socket& operator=(const Socket& other);

	SOCKET getWindowsSocket() { return windowsSocket; }
	char* getBuffer() { return socketBuffer; }
	char* getBufferAtPosition() { return socketBuffer + bufferPosition; }
	int getBufferSize() { return bufferSize; }
	int getBufferPosition() { return bufferPosition; }
	int getBufferAvailableSpace() { return bufferSize - bufferPosition; }
	
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

	void addRequest(HttpMessage* httpRequest) { httpRequests.push(httpRequest); }
	HttpMessage* getRequest();
};
