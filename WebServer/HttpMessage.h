#pragma once

#include "NetworkException.h"

class HttpMessage
{
private:

	enum class HttpMethod {
		NONE,
		GET,
		POST,
		PUT,
		OPTIONS,
		HEAD,
		HTTP_DELETE,
		TRACE
	};

	const std::unordered_map<std::string, HttpMethod> stringToMethod =
	{ {"GET",HttpMethod::GET} ,
		{"POST",HttpMethod::POST} ,
		{"PUT",HttpMethod::PUT} ,
		{"OPTIONS",HttpMethod::OPTIONS} ,
		{"HEAD",HttpMethod::HEAD} ,
		{"DELETE",HttpMethod::HTTP_DELETE} ,
		{"TRACE",HttpMethod::TRACE} };

	HttpMethod httpMethod;
	std::string httpVersion;
	std::string requestPath;
	std::string responseMessage;
	int statusCode;
	std::unordered_map<std::string, std::string> httpHeaders;
	int contentLength;
	std::string httpBody;

	void setHttpMethod(std::string& methodString);
	void setRequestPath(std::string& requestPath);
	void setHttpVersion(std::string& httpVersion);

	void setHttpHeaders(std::stringstream& requestString);
public:

	HttpMessage();

	static HttpMessage* buildRequest(char* buffer, int position);
	static HttpMessage* buildResponse(char* buffer, int position);
};

