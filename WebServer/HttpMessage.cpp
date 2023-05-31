#include "HttpMessage.h"

void HttpMessage::setHttpMethod(std::string& methodString)
{
	httpMethod = stringToMethod.at(methodString);
}

void HttpMessage::setRequestPath(std::string& requestPath)
{
	this->requestPath = requestPath;
}

void HttpMessage::setHttpVersion(std::string& httpVersion)
{
	this->httpVersion = httpVersion;
}

void HttpMessage::setStatusCode(int code)
{
	this->statusCode = code;
}

void HttpMessage::setResponseMessage(std::string& response)
{
	this->responseMessage = response;
}

void HttpMessage::setResponseBody(std::string& response)
{
	this->httpBody = response;
}

void HttpMessage::setHttpHeaders(std::stringstream& requestString)
{
	std::string key, value;
	do 
	{
		std::getline(requestString, key, ' ');
		std::getline(requestString, value);
		if (key != "")
			httpHeaders[key] = value;
	} while (key != "");
}

HttpMessage::HttpMessage()
{
	httpMethod = HttpMethod::GET;
	statusCode = contentLength = 0;
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
	std::getline(requestString, value, ' ');
	newMessage->setHttpVersion(value);
	newMessage->setHttpHeaders(requestString);


	return newMessage;
}

HttpMessage* HttpMessage::buildResponse(char* buffer, int position)
{
	return nullptr;
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