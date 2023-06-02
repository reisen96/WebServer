#pragma once

#include "NetworkException.h"

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

class HttpMessage
{
private:

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

	void setHttpHeadersAndBody(std::stringstream& requestString);
	
public:

	HttpMessage();

	static HttpMessage* buildRequest(char* buffer, int position);
	int writeToBuffer(char* buffer);

	HttpMethod getHttpMethod() { return httpMethod; }
	std::string& getHttpBody() { return httpBody; }

	void initializeResponseHeaders();

	void setStatusCode(int statusCode) { this->statusCode = statusCode; }
	void setHttpMethod(std::string& methodString) { httpMethod = stringToMethod.at(methodString); }
	void setHttpMethod(HttpMethod httpMethod) { this->httpMethod = httpMethod; }
	void setRequestPath(std::string& requestPath) { this->requestPath = requestPath; }
	void setHttpVersion(std::string& httpVersion) { this->httpVersion = httpVersion; }
	void setResponseMessage(const std::string& responseMessage) { this->responseMessage = responseMessage; }
	void setResponseBody(std::string& httpBody) { this->httpBody = httpBody; }
};

