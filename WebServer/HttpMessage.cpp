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
	std::getline(requestString, httpBody, '\0');
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
	int messageSize = 0;
	for (char ch : httpVersion)
		buffer[messageSize++] = ch;
	buffer[messageSize++] = ' ';
	for (char ch : std::to_string(statusCode))
		buffer[messageSize++] = ch;
	buffer[messageSize++] = ' ';
	for (char ch : responseMessage)
		buffer[messageSize++] = ch;
	buffer[messageSize++] = '\r';
	buffer[messageSize++] = '\n';
	for (std::pair<std::string, std::string> header : httpHeaders)
	{
		for (char ch : header.first)
			buffer[messageSize++] = ch;
		buffer[messageSize++] = ':';
		buffer[messageSize++] = ' ';
		for (char ch : header.second)
			buffer[messageSize++] = ch;
		buffer[messageSize++] = '\r';
		buffer[messageSize++] = '\n';
	}
	for (char ch : httpBody)
		buffer[messageSize++] = ch;
	buffer[messageSize++] = '\r';
	buffer[messageSize++] = '\n';
	buffer[messageSize + 1] = '\0';
	return messageSize;
}