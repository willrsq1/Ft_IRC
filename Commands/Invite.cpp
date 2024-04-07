#include "Commands.hpp"

void	invite(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 3) // not enough parameters
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
		return ;
	}

	if (commands.size() > 3) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1].empty()) // no nickname
	{
		server.sendToClient(client.getFd(), ERR_NORECIPIENT(client.getNickname(), "(INVITE)"));
		return ;
	}

	if (commands[2].empty() || commands[2][0] != '#') // no channel
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), "(INVITE)"));
		return ;
	}

	//channel existence check

	Channel * channel = NULL;

	for (it_channels it = server.getChannelsBegin() ; it != server.getChannelsEnd(); it ++)
	{
		if (it->first == commands[2])
		{
			channel = it->second;
			break ;
		}
	}

	if (channel == NULL)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[2]));
		return ;
	}

	//check if client is in channel

	if (channel->isClientInChannel(client.getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
		return ;
	}

	if (channel->isInviteOnly() == true && channel->isOperator(client.getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
		return ;
	}

	//client existence check

	Client * target = NULL;

	for (it_clients it = server.getClientsBegin(); it != server.getClientsEnd(); it++)
	{
		if (it->second->getNickname() == commands[1])
		{
			target = it->second;
			break ;
		}
	}

	if (target == NULL)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), commands[1]));
		return ;
	}

	//client already in channel check

	if (channel->isClientInChannel(target->getFd()) == true)
	{
		server.sendToClient(client.getFd(), ERR_USERONCHANNEL(client.getNickname(), target->getNickname(), channel->getName()));
		return ;
	}

	//client already invited check

	if (channel->isClientInvited(target->getFd()) == true)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("This user is already invited"));
		return ;
	}

	//end of checks

	channel->addInvitedUser(target->getFd());
	server.sendToClient(client.getFd(), RPL_INVITING(client.getNickname(), target->getNickname(), channel->getName()));
	server.sendToClient(target->getFd(), INVITE(client.getNickname(), client.getUsername(), target->getNickname(), channel->getName()));

}