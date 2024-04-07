#include "Commands.hpp"

void ModeOPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() != 4) // but parameter is required ! (MODE TYPE C)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("You must specify a parameter (nickname of the target)"));
		return ;
	}

	Client * target = server.getClientFromNickname(commands[3]);

	if (target == NULL)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), commands[3]));
		return ;
	}

	if (channel->isClientInChannel(target->getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("This user is not in the channel"));
		return ;
	}

	if (channel->isOperator(target->getFd()) == true)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("This user is already an operator"));
		return ;
	}

	channel->setNewOperator(target->getFd());
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "+o " + target->getNickname()));

}

void ModeOMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() != 4) //no param, but parameter is required ! (MODE TYPE C)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("You must specify a parameter (nickname of the target)"));
		return ;
	}

	Client * target = server.getClientFromNickname(commands[3]);

	if (target == NULL)
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), commands[3]));
		return ;
	}

	if (channel->isClientInChannel(target->getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("This user is not in the channel"));
		return ;
	}

	if (channel->isOperator(target->getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("This user is not an operator"));
		return ;
	}

	if (channel->getNbOperators() == 1)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("You can't remove the last operator"));
		return ;
	}

	channel->removeOperator(target->getFd());
	server.sendToClient(target->getFd(), RPL_UMODEIS(target->getNickname(), "-o"));
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "-o " + target->getNickname()));

}