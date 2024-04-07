#ifndef INPUTS_HPP
# define INPUTS_HPP	

#include <iostream>
#include <sstream>
#include <algorithm>

#include "../Includes/Server.hpp"

void	join(Server & server, Client & client, std::vector<std::string> & commands);
void	pass(Server & server, Client & client, std::vector<std::string> & commands);
void	nick(Server & server, Client & client, std::vector<std::string> & commands);
void	user(Server & server, Client & client, std::vector<std::string> & commands);
void	privmsg(Server & server, Client & client, std::vector<std::string> & commands);
void	part(Server & server, Client & client, std::vector<std::string> & commands);
void	kick(Server & server, Client & client, std::vector<std::string> & commands);
void	motd(Server & server, Client & client, std::vector<std::string> & commands);
void	list(Server & server, Client & client, std::vector<std::string> & commands);
void	ping(Server & server, Client & client, std::vector<std::string> & commands);
void	quit(Server & server, Client & client, std::vector<std::string> & commands);
void	invite(Server & server, Client & client, std::vector<std::string> & commands);
void	topic(Server & server, Client & client, std::vector<std::string> & commands);
void	lusers(Server & server, Client & client, std::vector<std::string> & commands);
void	mode(Server & server, Client & client, std::vector<std::string> & commands);

void    ModeLPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);
void    ModeLMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);

void 	ModeOPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);
void 	ModeOMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);

void    ModeIPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);
void    ModeIMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);

void    ModeTPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);
void    ModeTMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);

void 	ModeKPlus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);
void	ModeKMinus(Server & server, Client & client, std::vector<std::string> & commands, Channel * channel);

//utils

std::vector<std::string> split(std::string & str, char delim);
std::vector<std::string> basicSplit(std::string & str, char delim);
bool	caseInsensitiveCheck(std::string const & str, std::string const & str2);
std::string intToString(int nb);
std::string	getCurrentDateTime();


#endif