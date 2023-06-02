#include "HttpMessage.h"

HttpMessage::HttpMessage()
{
	httpMethod = HttpMethod::GET;
	httpVersion = "HTTP/1.1";
	statusCode = contentLength = 0;
}

void HttpMessage::setHttpHeadersAndBody(std::stringstream& requestString)
{
	std::string key, value;
	std::getline(requestString, value);
	do 
	{
		if (requestString.str()[requestString.tellg()] != '\r') 
		{
			std::getline(requestString, key, ' ');
			key[key.length() - 1] = '\0';
			std::getline(requestString, value, '\r');
			httpHeaders[key] = value;
			std::getline(requestString, value);
		}
	} while (requestString.str()[requestString.tellg()] != '\r');
	std::getline(requestString, value);
	std::getline(requestString, httpBody);
}

void HttpMessage::initializeResponseHeaders()
{
	httpHeaders["Content-Type"] = "text/html";
	httpHeaders["Server"] = "EX3 CPP Server";
	httpHeaders["Content-Length"] = "0";
}

HttpMessage* HttpMessage::buildRequest(char* buffer, int position)
{
	std::string value;
	std::stringstream requestString(buffer + position);
	HttpMessage* newMessage = new HttpMessage;
	std::getline(requestString, value, ' ');
	newMessage->setHttpMethod(value);
	std::getline(requestString, value, ' ');
	newMessage->setRequestPath(value);
	std::getline(requestString, value, '\r');
	newMessage->setHttpVersion(value);
	newMessage->setHttpHeadersAndBody(requestString);
	return newMessage;
}

int HttpMessage::writeToBuffer(char* buffer)
{
	return 0;
}