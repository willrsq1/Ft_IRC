#include "Commands.hpp"

void    ModeIPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() > 3) //must have no params! (MODE TYPE D)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters: this mode doesn't take parameters"));
		return ;
	}

	if (channel->isInviteOnly() == true)
	{
		server.sendToClient(client.getFd(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "this mode is already set."));
		return ;
	}

	channel->setMode('i');
	channel->setInviteOnly(true);
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), commands[2]));
}

void    ModeIMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() > 3) //must have no params! (MODE TYPE D)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters: this mode doesn't take parameters"));
		return ;
	}
	
	if (channel->isInviteOnly() == false)
	{
		server.sendToClient(client.getFd(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "this mode is already unset."));
		return ;
	}

	channel->unsetMode('i');
	channel->setInviteOnly(false);
	channel->clearInvitedUsers();
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), commands[2]));
}