#include "Commands.hpp"

static void	sendmsg(Server & server, Client & client, std::vector<std::string> & commands, std::string const & target);

void	privmsg(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 3) // not enough parameters
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
		return ;
	}

	if (commands.size() > 3) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1].empty()) // no target
	{
		server.sendToClient(client.getFd(), ERR_NORECIPIENT(client.getNickname(), "(PRIVMSG)"));
		return ;
	}

	if (commands[2].empty()) // no text to send
	{
		server.sendToClient(client.getFd(), ERR_NOTEXTTOSEND(client.getNickname()));
		return ;
	}

	std::vector<std::string> tokens = basicSplit(commands[1], ','); // different targets

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{

		sendmsg(server, client, commands, *it); // send message to each target

	}

}

static void	sendmsg(Server & server, Client & client, std::vector<std::string> & commands, std::string const & target)
{
	if (target.empty()) // no target
	{
		server.sendToClient(client.getFd(), ERR_NORECIPIENT(client.getNickname(), "(PRIVMSG)"));
		return ;
	}

	if (target == BOT_NAME) // if the target is the bot
	{
		server.sendAnswerToBot(client.getFd(), commands[2]);
		return ;
	}

	if (target[0] == '$' && target.substr(1) == (std::string)SERVER) // if the target is the Server
	{
		server.sendToAllClients(PRIVMSG(client.getNickname(), client.getUsername(), target, commands[2]));
		return ;
	}
	else if (target[0] == '#') // if the target is a channel
	{
		for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++) // goes through all channels
		{
			if (it->first == target) // if the channel exists
			{
				//gestion d'erreur a faire (ban, kick, etc)
				if (it->second->isClientInChannel(client.getFd()) == false)
				{
					server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), target));
					return ;
				}
				if (it->second->isClientInChannel(client.getFd()))
				{
					server.sendToAllClientsInChannelExceptOne(client.getFd(), it->second->getName(), PRIVMSG((std::string)(it->second->isOperator(client.getFd()) ? "@" : "") + client.getNickname(), client.getUsername(), target, commands[2]));
					return ;
				}
				else
				{
					server.sendToClient(client.getFd(), ERR_CANNOTSENDTOCHAN(client.getNickname(), target));
					return ;
				}
			}
		}
	}
	else // if the target is a client
	{	
		for (it_clients it = server.getClientsBegin(); it != server.getClientsEnd(); it++) // goes through all clients
		{
			if (it->second->getNickname() == target) // if the client exists
			{
				server.sendToClient(it->second->getFd(), PRIVMSG(client.getNickname(), client.getUsername(), target, commands[2]));
				return ;
			}
		}
	}

	server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), target)); // if no target was found
	
}
