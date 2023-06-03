#include "HttpMessage.h"

HttpMessage::HttpMessage()
{
	httpMethod = HttpMethod::GET;
	methodString = "GET";
	httpVersion = "HTTP/1.1";
	statusCode = contentLength = 0;
	httpBody = "";
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
			key.pop_back();
			std::getline(requestString, value, '\r');
			httpHeaders[key] = value;
			std::getline(requestString, value);
		}
	} while (requestString.str()[requestString.tellg()] != '\r');
	std::getline(requestString, value);
	std::getline(requestString, httpBody, '\0');
}

void HttpMessage::httpHeadersAndBodyToBuffer(char* buffer, int& messageSize)
{
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
	buffer[messageSize++] = '\r';
	buffer[messageSize++] = '\n';
	if (httpBody != "")
		for (char ch : httpBody)
			buffer[messageSize++] = ch;
}

void HttpMessage::initializeResponseHeaders()
{
	httpHeaders["Content-Type"] = "text/html;charset=utf-8";
	httpHeaders["Server"] = "EX3 CPP Server";
	httpHeaders["Content-Length"] = "0";
}

HttpMessage* HttpMessage::buildRequest(char* buffer, int position)
{
	std::string value;
	std::stringstream requestString(buffer + position);
	HttpMessage* newMessage = new HttpMessage;
	std::getline(requestString, value, ' ');
	newMessage->methodString = value;
	newMessage->setHttpMethod(value);
	std::getline(requestString, value, ' ');
	newMessage->setRequestPath(value);
	std::getline(requestString, value, '\r');
	newMessage->setHttpVersion(value);
	newMessage->setHttpHeadersAndBody(requestString);
	return newMessage;
}

int HttpMessage::writeResponseToBuffer(char* buffer)
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
	httpHeadersAndBodyToBuffer(buffer, messageSize);
	return messageSize;
}

int HttpMessage::writeRequestToBuffer(char* buffer)
{
	int messageSize = 0;
	for (char ch : methodString)
		buffer[messageSize++] = ch;
	buffer[messageSize++] = ' ';
	for (char ch : requestPath)
		buffer[messageSize++] = ch;
	buffer[messageSize++] = ' ';
	for (char ch : httpVersion)
		buffer[messageSize++] = ch;
	buffer[messageSize++] = '\r';
	buffer[messageSize++] = '\n';
	httpHeadersAndBodyToBuffer(buffer, messageSize);
	return messageSize;
}

std::string HttpMessage::getRequestPath()
{
	size_t start = requestPath.find('/') + 1u, end = requestPath.find('?');
	return requestPath.substr(start, end - start);
}

std::string HttpMessage::getQueryParameter(const std::string& key)
{
	size_t start = requestPath.find(key) + key.length() + 1u, end = requestPath.find('&', start);
	return requestPath.substr(start, end - start);
}

void HttpMessage::setContentLength(int contentLength)
{
	this->contentLength = contentLength;
	httpHeaders["Content-Length"] = std::to_string(contentLength);
}
