#include "Commands.hpp"

void	quit(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() > 2)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters."));
		return ;
	}

	//envoyer a tous les clients dans des channels similaires un message de deconnection

	for (it_clients it = server.getClientsBegin(); it != server.getClientsEnd(); it++)
	{
		if (it->second->getIsRegistered() == true && it->second->getNickname() != client.getNickname())
		{
			Client *target = it->second;
			for (it_channels it2 = target->getChannelsBegin(); it2 != target->getChannelsEnd(); it2++)
			{
				if (it2->second->isClientInChannel(client.getFd()) == true)
				{
					if (commands[1].empty())
					{
						commands[1] = "No reason given";
					}
					server.sendToClient(target->getFd(), QUIT(client.getNickname(), client.getUsername(), commands[1]));
					break ;
				}
			}
		}
	}

	server.sendToClient(client.getFd(), ERR_FATALERROR("Goodbye!"));

	server.disconnectClient(client);
}