#include "Commands.hpp"

void	motd(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() > 2)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters for command MOTD."));
		return ;
	}
	if (commands.size() == 2 && commands[1] != SERVER)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHSERVER(commands[1]));
		return ;
	}
	server.sendToClient(client.getFd(), RPL_MOTDSTART(client.getNickname()));
	server.sendToClient(client.getFd(), RPL_MOTD(client.getNickname(), "Welcome to the our IRC serv. Will you be the first to reach 100 points ? -"));
	server.sendToClient(client.getFd(), RPL_MOTD(client.getNickname(), "Good luck ! -"));
	server.sendToClient(client.getFd(), RPL_ENDOFMOTD(client.getNickname()));
}