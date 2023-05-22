#include "Client.h"

void Client::getRequest()
{
	if (MESSAGE == "POST")
	{
		readPointer = 6;
		HTTP_METHOD = POST;
	}

	if (MESSAGE == "GET")
	{
		readPointer = 5;
		HTTP_METHOD = GET;
	}

	if (MESSAGE == "PUT")
	{
		readPointer = 5;
		HTTP_METHOD = PUT;
	}

	if (MESSAGE == "DELETE")
	{
		readPointer = 8;
		HTTP_METHOD = H_DELETE;
	}
	
	if (MESSAGE == "HEAD")
	{
		readPointer = 6;
		HTTP_METHOD = HEAD;
	}

	if (MESSAGE == "OPTIONS")
	{
		readPointer = 9;
		HTTP_METHOD = OPTIONS;
	}
	
	if (MESSAGE == "TRACE")
	{
		readPointer = 7;
		HTTP_METHOD = TRACE;
	}

	if (MESSAGE != "POST" && MESSAGE != "GET" && MESSAGE != "PUT" && 
		MESSAGE != "DELETE" && MESSAGE != "HEAD" && MESSAGE != "OPTIONS" && MESSAGE != "TRACE")
	{
		HTTP_METHOD = UNSUPPORTED;
		readPointer = 0;
	}
}

void Client::getHeader()
{

}

void Client::cleanRequest()
{
	HEADER_CONTENT_LENGTH = 0;
	readPointer = 0;
	HEADER_HOST.clear();
	HEADER_CONNECTION.clear();
	HEADER_ACCEPT.clear();
	HEADER_ACCEPT_LANGUAGE.clear();
	HEADER_ACCEPT_SET.clear();
	HEADER_CONTENT_TYPE.clear();
	MESSAGE.clear();
	HOST.clear(); 
	HTTP_METHOD = NONE;
	HTTP_ERROR = NOERR;
	HTTP_QUERY.clear(); 
	HTTP_BODY.clear();
	PATH.clear();
	DATE.clear();
	RESPONSE_MESSAGE.clear();
	BODY_HANDLER.clear();
}

void Client::cleanResponse()
{
	RESPONSE_MESSAGE = { "HTTP/1.1 " };
}