#pragma once

#include "NetworkException.h"

class HttpMessage
{
private:

	enum class HttpMethod {
		GET,
		POST,
		PUT,
		OPTIONS,
		HEAD,
		HTTP_DELETE,
		TRACE,
		NONE
	};

	HttpMethod httpMethod;
	std::string httpVersion;
	std::string requestPath;
	std::string responseMessage;
	int statusCode;
	std::unordered_map<std::string, std::string> httpHeaders;
	int contentLength;
	std::string httpBody;

	void setHttpMethod(std::string& methodString);

public:

	HttpMessage();

	static HttpMessage* buildRequest(char* buffer, int position);
	static HttpMessage* buildResponse(char* buffer, int position);
};

