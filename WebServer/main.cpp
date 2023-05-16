#include "WebServer.h"

int main(int argc, char* argv[]) {

	try
	{
		WebServer webServer;
		webServer.run();
	}
	catch (std::exception exception) 
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}