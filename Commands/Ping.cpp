#include "Commands.hpp"

void	ping(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() > 2)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters."));
		return ;
	}

	if (commands.size() == 1 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NOORIGIN(client.getNickname()));
		return ;
	}
	
	server.sendToClient(client.getFd(), PONG(client.getNickname(), commands[1]));
}