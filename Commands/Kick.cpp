#include "Commands.hpp"

void	kickTheUser(Server & server, Client & client, std::vector<std::string> & commands, Channel & channel);

void	kick(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 3 || commands[1].empty() || commands[2].empty())
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
		return ;
	}

	if (commands.size() > 4)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++)
	{
		if (it->second->getName() == commands[1])
		{
			kickTheUser(server, client, commands, *(it->second));
			return ;
		}
	}

	server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1]));
}

void	kickTheUser(Server & server, Client & client, std::vector<std::string> & commands, Channel & channel)
{
	if (channel.isClientInChannel(client.getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
		return ;
	}

	if (channel.isOperator(client.getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel.getName()));
		return ;
	}

	std::vector<std::string> tokens = basicSplit(commands[2], ','); // different targets

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		Client * target = server.getClientFromNickname(*it);
		
		if (target == NULL)
		{
			server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), *it));
		}
		else if (channel.isClientInChannel(target->getFd()) == false)
		{
			server.sendToClient(client.getFd(), ERR_USERNOTINCHANNEL(client.getNickname(), *it, channel.getName()));
		}
		else
		{
			std::string reason;

			if (commands.size() == 4)
			{
				reason = commands[3];
			}
			else
			{
				reason = "Kicked by " + client.getNickname();
			}
			channel.removeClient(target->getFd());
			channel.addBannedUser(target->getFd());
			server.sendToClient(target->getFd(), KICK(client.getNickname(), client.getUsername(), channel.getName(), target->getNickname(), reason));
			target->removeChannel(channel.getName());
			server.sendToAllClientsInChannel(channel.getName(), KICK(client.getNickname(), client.getUsername(), channel.getName(), target->getNickname(), reason));
		}

	}

	}