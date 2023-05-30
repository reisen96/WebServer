#include "HttpMessage.h"

void HttpMessage::setHttpMethod(std::string& methodString)
{
	if (methodString == "GET")
		httpMethod = HttpMethod::GET;
}

HttpMessage::HttpMessage()
{
	httpMethod = HttpMethod::GET;
	statusCode = contentLength = 0;
}

HttpMessage* HttpMessage::buildRequest(char* buffer, int position)
{
	std::string left, right;
	std::stringstream requestString(buffer + position);
	HttpMessage* newMessage = new HttpMessage;
	std::getline(requestString, left, ' ');
	newMessage->setHttpMethod(left);


	return newMessage;
}

HttpMessage* HttpMessage::buildResponse(char* buffer, int position)
{
	return nullptr;
}
