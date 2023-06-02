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

}

void HttpMessage::generateResponseForGET() {

}

void HttpMessage::generateResponseForPOST() {

}
void HttpMessage::generateResponseForPUT() {

}
void HttpMessage::generateResponseForOPTIONS() {
	std::string answer = "Our options are: GET, POST, PUT, OPTIONS, HEAD, DELETE, TRACE";
	this->setStatusCode(200);
	this->setResponseBody(answer);
}
void HttpMessage::generateResponseForHEAD() {

}
void HttpMessage::generateResponseForDELETE() {

}
void HttpMessage::generateResponseForTRACE() {

}
void HttpMessage::generateResponseForINVALID() {
	std::string answer = "Invalid option!";
	this->setResponseBody(answer);
}