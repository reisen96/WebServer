#include "WebServer.h"

WebServer::WebServer() 
{
	serverSocket.initialize("", serverPort, SOCK_STREAM, IPPROTO_TCP);
	serverSocket.bindToPort();
	serverSocket.setListen(serverBacklog);
}

void WebServer::run()
{
	while (true) 
	{

	}
}


