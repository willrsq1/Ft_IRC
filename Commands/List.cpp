#include "Commands.hpp"

void	list(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() > 2)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters for command LIST. ELIST=''"));
		return ;
	}

	if (commands.size() < 2 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), RPL_LISTSTART(client.getNickname()));
		for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); ++it)
		{
			std::string nbClients = intToString(it->second->getNbClients());
			server.sendToClient(client.getFd(), RPL_LIST(client.getNickname(), it->second->getName(), nbClients, it->second->getTopic()));
		}
		server.sendToClient(client.getFd(), RPL_LISTEND(client.getNickname()));
		return ;
	}

	std::vector<std::string> tokens = split(commands[1], ',');

	server.sendToClient(client.getFd(), RPL_LISTSTART(client.getNickname()));

	bool channelFound = false;

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		for (it_channels it2 = server.getChannelsBegin(); it2 != server.getChannelsEnd(); ++it2)
		{
			if (it2->second->getName() == *it)
			{
				std::string nbClients = intToString(it2->second->getNbClients());
				server.sendToClient(client.getFd(), RPL_LIST(client.getNickname(), it2->second->getName(), nbClients, it2->second->getTopic()));
				channelFound = true;
				break ;
			}
		}
		if (channelFound == false)
		{
			server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), *it));
		}
		channelFound = false;
	}

	server.sendToClient(client.getFd(), RPL_LISTEND(client.getNickname()));

}
