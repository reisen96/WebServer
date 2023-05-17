#pragma once

#include "NetworkException.h"
#include "Client.h"

constexpr int bufferSize = 255;

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
	Client client;
	sockaddr_in socketAddress;
	SocketState socketState;

	char socketBuffer[bufferSize];

	Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress, SocketState socketState = SocketState::Inactive);
	
public:

	Socket();
	~Socket();

	SOCKET getWindowsSocket() { return windowsSocket; }
	char* getBuffer() { return socketBuffer; }
	int getBufferSize() { return bufferSize; }

	char& operator[](int index);

	Socket acceptConnection();

	bool isInactive() { return socketState == SocketState::Inactive; }
	bool listenState() { return socketState == SocketState::Listen; }
	bool receiveState() { return socketState == SocketState::Receive; }
	bool sendState() { return socketState == SocketState::Send; }
	Client getClient() { return client; }

	void initialize(const std::string& ipAddress, unsigned short port, int type = SOCK_DGRAM, int protocol = IPPROTO_UDP);

	void bindToPort();
	void setListen(int backlog = 0);
	void setMode(bool blocking);
	void setReceive() { socketState = SocketState::Receive; }
	void setSend() { socketState = SocketState::Send; }
	void setInactive() { socketState = SocketState::Inactive; }
	void close();
};
