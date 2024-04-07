#include "Commands.hpp"

void	joinChannel(Server & server, Client & client, std::string & token, std::string key);
void	newClientInChannel(Server & server, Client & client, Channel & channel, std::string key);

void	join(Server & server, Client & client, std::vector<std::string> & commands)
{
	
	if (commands.size() < 2 || commands[1].empty()) // no parameters
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return ;
	}

	if (commands.size() > 3) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1].size() == 1 && commands[1][0] == '#') // its just "#"
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid channel name. Cannot create channel."));
		return ;
	}

	if (commands[1] == "0") //part from all channels
	{
		for (it_channels it = client.getChannelsBegin(); it != client.getChannelsEnd(); it++)
		{
			server.sendToAllClientsInChannel(it->first, PART(client.getNickname(), client.getUsername(), it->first, "Leaving"));
			it->second->removeClient(client.getFd());
		}

		client.leaveAllChannels();

		return ;
	}

	std::vector<std::string> keys;
	
	if (commands.size() == 3)
	{
		keys = basicSplit(commands[2], ',');
	}

	std::vector<std::string>::iterator itKey = keys.begin();

	std::vector<std::string> tokens = basicSplit(commands[1], ','); // different targets

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{

		if (client.getNbChannels() > CHANLIMIT)
		{
			server.sendToClient(client.getFd(), ERR_TOOMANYCHANNELS(client.getNickname(), commands[1]));
			return ;
		}

		std::string key = "";
		if (itKey != keys.end())
		{
			key = *itKey;
			itKey++;
		}
		joinChannel(server, client, *it, key); // send message to each target

	}

}

void	joinChannel(Server & server, Client & client, std::string & token, std::string key)
{
	
	if (token[0] != '#') // its not a channel	
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), token));
		return ;
	}

	if (token.find(' ') != std::string::npos || token.find(0x07) != std::string::npos || token.find(',') != std::string::npos) // invalid characters from RFC
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid characters in channel name. Cannot create channel."));
		return ;
	}


	for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++)
	{

		if (caseInsensitiveCheck(it->first, token)) //does exist (CASE INSENSITIVE)
		{
			if (it->second->isClientInChannel(client.getFd())) //if client already in channel
			{
				server.sendToClient(client.getFd(), ERR_FATALERROR("You are already in this channel"));
				return ;
			}
			if (it->second->isBanned(client.getFd())) //if client is banned
			{
				server.sendToClient(client.getFd(), ERR_FATALERROR("You are banned from this channel"));
				return ;
			}
			newClientInChannel(server, client, *it->second, key); //if not, add client to channel
			return ;
		}

	}

	//if channel does not exist, create it and add the client to it

	Channel * channel = new Channel(token, &server);
	server.addChannel(token, channel); // register the new channel in the server !
	newClientInChannel(server, client, *channel, key);

}

void	newClientInChannel(Server & server, Client & client, Channel & channel, std::string key)
{
	//add the client to the channel, and the channel to the client

	if (channel.isSetMode('i') && channel.isClientInvited(client.getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_INVITEONLYCHAN(client.getNickname(), channel.getName()));
		return ;
	}

	if (channel.isSetMode('k') && channel.getKey() != key)
	{
		server.sendToClient(client.getFd(), ERR_BADCHANNELKEY(client.getNickname(), channel.getName()));
		return ;
	}

	if (channel.limitIsReached() == true)
	{
		server.sendToClient(client.getFd(), ERR_CHANNELISFULL(client.getNickname(), channel.getName()));
		return ;
	}

	channel.addClient(client.getFd(), &client);
	client.addChannel(channel.getName(), &channel);
	server.sendToAllClientsInChannel(channel.getName(), JOIN(client.getNickname(), client.getUsername(), channel.getName()));

	if (channel.getTopic().empty())
	{
		server.sendToClient(client.getFd(), RPL_NOTOPIC(client.getNickname(), channel.getName()));
	}
	else
	{
		server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
		server.sendToClient(client.getFd(), RPL_TOPICWHOTIME(client.getNickname(), channel.getName(), channel.getTopicEditor(), channel.getTopicDate()));
	}

	for (it_clients it = channel.getClientsBegin(); it != channel.getClientsEnd(); it++)
	{
		std::string prefix = "";
		if (channel.isOperator(it->second->getFd()))
		{
			prefix = "@";
		}
		server.sendToClient(client.getFd(), RPL_NAMREPLY(client.getNickname(), channel.getName(), prefix, it->second->getNickname()));
	}

	server.sendToClient(client.getFd(), RPL_ENDOFNAMES(client.getNickname(), channel.getName()));

}

