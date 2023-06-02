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

	void setHttpBody(std::stringstream& requestString);

public:

	HttpMessage();

	HttpMethod getHttpMethod() { return httpMethod; }
	static HttpMessage* buildRequest(char* buffer, int position);
	static HttpMessage* buildResponse(char* buffer, int position);

	void setStatusCode(int code);
	void setHttpMethod(std::string& methodString);
	void setRequestPath(std::string& requestPath);
	void setHttpVersion(std::string& httpVersion);
	void setResponseMessage(std::string& response);
	void setResponseBody(std::string& response);
	void setHttpHeaders(std::stringstream& requestString);


	void generateResponseForGET();
	void generateResponseForPOST();
	void generateResponseForPUT();
	void generateResponseForOPTIONS();
	void generateResponseForHEAD();
	void generateResponseForDELETE();
	void generateResponseForTRACE();
	void generateResponseForINVALID();
};

