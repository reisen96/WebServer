#include "WebServer.h"

int main(int argc, char* argv[]) {

	try
	{
		WebServer webServer;
		webServer.run();
	}
	catch (const std::exception exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}