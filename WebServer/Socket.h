#pragma once

#include "NetworkException.h"

class Socket
{
private:

	const int majorWinsockVersion = 2;
	const int minorWinsockVersion = 2;
	SOCKET windowsSocket;
	sockaddr_in socketAddress;

	Socket(SOCKET& windowsSocket, sockaddr_in& socketAddress);
	
public:

	~Socket();

	SOCKET getWindowsSocket() { return windowsSocket; }

	Socket acceptConnection();

	void initialize(const std::string& ipAddress, unsigned short port, int type = SOCK_DGRAM, int protocol = IPPROTO_UDP);
	void bindToPort();
	void listenState(int backlog = 0);
	void setMode(bool blocking);
	void close();
};

