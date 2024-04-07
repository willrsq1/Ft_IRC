#include "Commands.hpp"

void    ModeLPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() != 4) //no param, but parameter is required ! (MODE TYPE C)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid mode string, you must specify a parameter (positive int between 1 and 10000)"));
		return ;
	}

	//check if parameter is an int

	for (size_t i = 0; i < commands[3].size() && i < 7; i++)
	{
		if (commands[3][i] < '0' || commands[3][i] > '9')
		{
			server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid mode string, you must specify a parameter (positive int between 1 and 10000)"));
			return ;
		}
	}

	//check if parameter is between 1 and 10000

	int limit = std::atoi(commands[3].c_str());

	if (limit < 1 || limit > 10000)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid mode string, you must specify a parameter (positive int between 1 and 10000)"));
		return ;
	}

	commands[3] = commands[2] + ": limit is now: " + commands[3];

	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), commands[3]));

	channel->setMode('l');
	channel->setLimit(true);
	channel->setLimitValue(limit);

}

void    ModeLMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands.size() != 3)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid input, you shouldn't specify a parameter"));
		return ;
	}

	if (channel->isSetMode('l') == false)
	{
		server.sendToClient(client.getFd(), MODE(client.getNickname(), client.getUsername(), channel->getName(), "this mode is already unset."));
		return ;
	}

	channel->unsetMode('l');
	channel->setLimit(false);
	channel->setLimitValue(0);
	server.sendToAllClientsInChannel(channel->getName(), MODE(client.getNickname(), client.getUsername(), channel->getName(), commands[2]));
}
