#include "Commands.hpp"

void ModeKPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	//must have 4 params

	if (commands.size() < 4 || commands[3].empty())
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "You must have a key as the last parameter."));
		return ;
	}

	if (commands.size() > 4)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("You can only have 4 parameters."));
		return ;
	}

	//check that the key is only letters and numbers

	for (size_t i = 0; i < commands[3].size(); i++)
	{
		if (!isalnum(commands[3][i]))
		{
			server.sendToClient(client.getFd(), ERR_INVALIDMODEPARAM(client.getNickname(), commands[1], commands[2], commands[3], "Key must be alphanumeric."));
			return ;
		}
	}

	if (channel->hasAKey())
	{
		channel->setKey(commands[3]);
		channel->setMode('k');
		server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "+k The key has been changed to " + commands[3]));
		return ;
	}
	else
	{
		server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "+k The key has been set to " + commands[3]));
		channel->setKey(commands[3]);
		channel->setMode('k');
		return ;
	}
}

void ModeKMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	
	if (channel->hasAKey() == false)
	{
		server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "-k The key is already unset"));
		return ;
	}

	if (commands.size() < 4 || commands[3].empty())
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "You must have the current key as the last parameter."));
		return ;
	}

	if (commands.size() > 4)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("You can only have 4 parameters."));
		return ;
	}

	if (commands[3] != channel->getKey())
	{
		server.sendToClient(client.getFd(), ERR_INVALIDKEY(client.getNickname(), channel->getName()));
		return ;
	}

	channel->clearKey();
	channel->unsetMode('k');
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "-k"));
	return ;

}
