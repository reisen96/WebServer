#pragma once

#include "NetworkException.h"

class Socket
{
private:

	const int majorWinsockVersion = 2;
	const int minorWinsockVersion = 2;
	SOCKET windowsSocket;
	sockaddr_in socketAddress;
	
public:

	~Socket();

	SOCKET getWindowsSocket() { return windowsSocket; }

	void initialize(const std::string& ipAddress, unsigned short port, int type = SOCK_DGRAM, int protocol = IPPROTO_UDP);
	void bindToPort();
	void close();
};

