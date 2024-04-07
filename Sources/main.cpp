

#include "../Includes/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	signal(SIGINT, Server::mySigIntHandler);
	try
	{
		Server a(argv[1], argv[2]);
	}
	
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
}