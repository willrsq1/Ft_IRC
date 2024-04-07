#include "Commands.hpp"

void	topic(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 2)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
		return ;
	}

	if (commands.size() > 3)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1][0] != '#')
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1]));
		return ;
	}

	Channel * channel = NULL;

	for (it_channels it = server.getChannelsBegin() ; it != server.getChannelsEnd(); it ++)
	{
		if (it->first == commands[1])
		{
			channel = it->second;
			break ;
		}
	}

	if (channel == NULL)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1]));
		return ;
	}

	if (commands.size() == 2)
	{
		if (channel->isClientInChannel(client.getFd()) == false)
		{
			server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
			return ;
		}

		if (channel->getTopic().empty())
		{
			server.sendToClient(client.getFd(), RPL_NOTOPIC(client.getNickname(), channel->getName()));
			return ;
		}
		else
		{
			server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), channel->getName(), channel->getTopic()));
			server.sendToClient(client.getFd(), RPL_TOPICWHOTIME(client.getNickname(), channel->getName(), channel->getTopicEditor(), channel->getTopicDate()));
			return ;
		}
	}
	else
	{
		if (channel->isClientInChannel(client.getFd()) == false)
		{
			server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
			return ;
		}
		if (channel->isTopicProtected() == true && channel->isOperator(client.getFd()) == false)
		{
			server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel->getName()));
			return ;
		}
		else
		{
			if (commands[2].size() > TOPICLEN)
			{
				commands[2].resize(TOPICLEN);
				server.sendToClient(client.getFd(), ERR_FATALERROR("Your topic was resized to: " + commands[2]));
			}
			channel->setTopic(commands[2], client.getNickname());
			server.sendToAllClientsInChannel(channel->getName(), TOPIC(client.getNickname(), client.getUsername(), channel->getName(), commands[2]));
			return ;
		}
		
	}
}