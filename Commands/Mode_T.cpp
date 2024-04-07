#include "Commands.hpp"

void    ModeTPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() > 3) //must have no params! (MODE TYPE D)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters: this mode doesn't take parameters"));
		return ;
	}

	if (channel->isTopicProtected() == true)
	{
		server.sendToClient(client.getFd(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "this mode is already set."));
		return ;
	}

	channel->setMode('t');
	channel->setTopicProtected(true);
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), commands[2]));

}

void    ModeTMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() > 3) //must have no params! (MODE TYPE D)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters: this mode doesn't take parameters"));
		return ;
	}
	
	if (channel->isTopicProtected() == false)
	{
		server.sendToClient(client.getFd(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "this mode is already unset."));
		return ;
	}

	channel->unsetMode('t');
	channel->setTopicProtected(false);
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), commands[2]));

}