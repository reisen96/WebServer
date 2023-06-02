#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <queue>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>

class NetworkException : public std::exception
{
private:

	std::string errorMessage;

public:

	NetworkException(const std::string& message) { errorMessage = message; }
	const char* what() { return errorMessage.c_str(); }
};

