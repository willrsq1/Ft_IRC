#include "Commands.hpp"

void	ChecksForChannel(Server & server, Client & client, std::vector<std::string> & commands);
void	modeForChannel(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);

void	mode(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 2 || commands[1].empty()) // no parameters
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		return ;
	}

	if (commands.size() > 5) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1][0] == '#') // its not a channel	
	{
		ChecksForChannel(server, client, commands);
		return ;
	}
	else
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1]));
		return ;
	}

}

void	ChecksForChannel(Server & server, Client & client, std::vector<std::string> & commands)
{
	//channel existence check

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

	// end of existence check


	if (commands.size() == 2) // if not modestring
	{
		server.sendToClient(client.getFd(), RPL_CHANNELMODEIS(client.getNickname(), commands[1], channel->getMode()));
		return ;
	}

	if (channel->isOperator(client.getFd()) == false)
	{
		server.sendToClient(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNickname(), commands[1]));
		return ;
	}

	if (commands[2][0] != '+' && commands[2][0] != '-')
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid mode string, you must start with + or -"));
		return ;
	}

	if (commands[2].size() != 2)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid mode string, you must specify only one mode (+X or -X)"));
		return ;
	}

	if (commands[2][1] != 'l' && commands[2][1] != 'o' && commands[2][1] != 'k' && commands[2][1] != 't' && commands[2][1] != 'i')
	{
		server.sendToClient(client.getFd(), ERR_UNKNOWNMODE(client.getNickname(), commands[2][1]));
		return ;
	}

	modeForChannel(server, client, commands, channel);

}


void	modeForChannel(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel)
{
	if (commands[2][0] == '+')
	{
		if (commands[2][1] == 'l')
		{
			ModeLPlus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 'o')
		{
			ModeOPlus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 'k')
		{
			ModeKPlus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 't')
		{
			ModeTPlus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 'i')
		{
			ModeIPlus(server, client, commands, channel);
			return ;
		}
	}




	else if (commands[2][0] == '-')
	{
		if (commands[2][1] == 'l')
		{
			ModeLMinus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 'o')
		{
			ModeOMinus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 'k')
		{
			ModeKMinus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 't')
		{
			ModeTMinus(server, client, commands, channel);
			return ;
		}
		else if (commands[2][1] == 'i')
		{
			ModeIMinus(server, client, commands, channel);
			return ;
		}
	}
}
