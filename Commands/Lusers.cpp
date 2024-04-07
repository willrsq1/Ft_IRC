#include "Commands.hpp"

void	lusers(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() != 1)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	server.sendToClient(client.getFd(), RPL_LUSERCLIENT(client.getNickname(), server.getNbClients()));
	server.sendToClient(client.getFd(), RPL_LUSEROP(client.getNickname()));
	server.sendToClient(client.getFd(), RPL_LUSERUNKNOWN(client.getNickname(), server.getNbClientsUnregistered()));
	server.sendToClient(client.getFd(), RPL_LUSERCHANNELS(client.getNickname(), server.getNbChannels()));
	server.sendToClient(client.getFd(), RPL_LUSERME(client.getNickname(), server.getNbClients()));
	server.sendToClient(client.getFd(), RPL_LOCALUSERS(client.getNickname(), server.getNbClients(), server.getMaxClients()));
	server.sendToClient(client.getFd(), RPL_GLOBALUSERS(client.getNickname(), server.getNbClients(), server.getMaxClients()));
}
