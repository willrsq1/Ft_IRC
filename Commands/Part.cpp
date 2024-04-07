#include "Commands.hpp"

void	clientPartingFromChannel(Server & server, Client & client, std::vector<std::string> & commands);

void	part(Server & server, Client & client, std::vector<std::string> & commands)
{

	if (commands.size() < 2 || commands[1].empty()) // no parameters
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
		return ;
	}

	if (commands.size() > 3) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	std::vector<std::string> tokens = basicSplit(commands[1], ','); //get the list of channels

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{

		commands[1] = *it;
		clientPartingFromChannel(server, client, commands); //part from each channel !
		
	}

}

void	clientPartingFromChannel(Server & server, Client & client, std::vector<std::string> & commands)
{

	for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++) //look if channel already exists
	{

		if (it->first == commands[1]) //does exist, found
		{
			if (it->second->isClientInChannel(client.getFd())) //if client in channel, execute the PART command
			{
				server.sendToAllClientsInChannel(it->first, PART(client.getNickname(), client.getUsername(), commands[1], ((commands.size() == 3) ? commands[2] : "empty"))); //warn everyone in chan
				server.removeClientFromChannel(it->first, client.getFd()); //remove client from channel and channel from client -- deletes the channel if its empty, changes the operator if the operator left
				return ;
			}
			server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), commands[1])); //send error if client not in the specified channel
			return ;
		}
		else if (caseInsensitiveCheck(it->first, commands[1]))
		{
			server.sendToClient(client.getFd(), ERR_FATALERROR("A similar channel was found, but channel's names are case insensitive, try again with the good case !")); //channel does not exist but could be a bad case input
			return ; //No channel with this name can exist, so we return
		}

	}

	server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1])); //channel does not exist
}