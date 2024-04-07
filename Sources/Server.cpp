#include "../Includes/Server.hpp"

Server::Server(){}

bool Server::running = true;

Server::Server(std::string const & port, std::string const & password):shutting_down(false), password(password), nbSockets(0), maxClients(0)
{
	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos)
	{
		throw std::invalid_argument("Error: Wrong port format");
	}

	std::istringstream token(port);
	int portNumber;
	token >> portNumber;

	if (token.fail() || portNumber < 1 || portNumber > 65535)
	{
		throw std::invalid_argument("Error: Wrong port format");
	}
	if (password.empty())
	{
		throw std::invalid_argument("Error: Password cannot be empty");
	}

	this->password = password;
	
	registerDateCreation();
	registerCommand();
	createMySocket(portNumber);
	bot.setServer(this);
	loop();
}

void	Server::registerCommand()
{
	commands["PASS"] = &pass;
	commands["NICK"] = &nick;
	commands["USER"] = &user;
	commands["PRIVMSG"] = &privmsg;
	commands["JOIN"] = &join;
	commands["PART"] = &part;
	commands["KICK"] = &kick;
	commands["MOTD"] = &motd;
	commands["LIST"] = &list;
	commands["PING"] = &ping;
	commands["QUIT"] = &quit;
	commands["INVITE"] = &invite;
	commands["TOPIC"] = &topic;
	commands["LUSERS"] = &lusers;
	commands["MODE"] = &mode;
}

Server::~Server()
{
	
	for (int i = 0; i < nbSockets; i++)
	{
		close(pollfds[i].fd);
	}

	shutting_down = true;

	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		delete it->second;
	}

	for (it_channels it = channels.begin(); it != channels.end(); it++)
	{
		delete it->second;
	}
}

void Server::createMySocket(int port)
{
	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd == -1)
		throw std::runtime_error("Error: socket creation failed");

	int yes = 1;
	if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		close (serverSocketFd);
		throw std::runtime_error("Error: setsockopt(SO_REUSEADDR) failed");
	}
	
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(serverSocketFd, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		close (serverSocketFd);
		throw std::runtime_error("Error: bind failed");
	}

	if (listen(serverSocketFd, SOMAXCONN) == -1)
	{
		close (serverSocketFd);
		throw std::runtime_error("Error: listen failed");
	}

	pollfds[nbSockets].fd = serverSocketFd;
	pollfds[nbSockets].events = POLLIN;
	nbSockets++;
	std::cout << "Server is listening on port " << port << std::endl;
	
}

void	Server::loop()
{
	while (running == true)
	{
		bot.update();
		int ret = poll(pollfds, nbSockets, 1000);
		if (ret == -1)
		{
			if (running == false)
				break ;
			throw std::runtime_error("Error: poll failed");
		}
		else if (ret == 0)
			continue ;
		for (int i = 0; i < nbSockets; i++)
		{
			if ((pollfds[i].revents & POLLIN) != 0)
			{
				//fct qui check erreur de revent
				if (i == 0)
				{
					newClient();
				}
					//serv
				else
				{
					newMessage(i);
				}
					//client
			}
		}
	}
	std::cerr << "Server is shutting down" << std::endl;
}



void	Server::newClient()
{
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);

	int clientSocketFd = accept(pollfds[0].fd, (sockaddr *)&clientAddress, &clientAddressSize);
	if (clientSocketFd == -1)
		throw std::runtime_error("Error: accept failed");
	
	pollfds[nbSockets].fd = clientSocketFd;
	pollfds[nbSockets].events = POLLIN;
	pollfds[nbSockets].revents = 0;


	clients[clientSocketFd] = new Client(clientSocketFd);
	clients[clientSocketFd]->setiPoll(nbSockets);
	std::cout << "New client connected :" << clients[clientSocketFd]->getNickname() << std::endl;
	nbSockets++;
	if (nbSockets - 1 > maxClients)
		maxClients = nbSockets;
}



void	Server::newMessage(int i)
{
	char	buff[BUFFER_SIZE];
	std::string & socketBuff = clients[pollfds[i].fd]->getSocketBuff();

	int ret = recv(pollfds[i].fd, buff, sizeof(buff), 0);
	if (ret == -1)
		throw std::runtime_error("Error: recv failed");
	if (ret == 0)
	{
		std::vector<std::string> tokens;
		tokens.push_back("QUIT");
		tokens.push_back("");
		quit(*this, *clients[pollfds[i].fd], tokens);
		return ;
	}
	else if (ret == 512)
	{
		sendToClient(pollfds[i].fd, ERR_INPUTTOOLONG(clients[pollfds[i].fd]->getNickname()));
		return ;
	}
	buff[ret] = '\0';

	socketBuff += buff;

	if (socketBuff.empty())
		return ;

	size_t pos = socketBuff.find("\r\n");
	if (pos == std::string::npos)
		pos = socketBuff.find("\n");
	while (pos != std::string::npos)
	{
		std::string command = socketBuff.substr(0, pos);
		socketBuff.erase(0, pos + 2);
		commandExecution(command, pollfds[i].fd);
		if (clients.find(pollfds[i].fd) == clients.end())
			return ;
		pos = socketBuff.find("\r\n");
		if (pos == std::string::npos)
			pos = socketBuff.find("\n");
	}
	
}

void	Server::commandExecution(std::string & command, int clientSocket)
{
	std::vector<std::string> tokens = split(command, ' ');

	std::cout << "Message from client nb [" << clientSocket - 3 << "] : " << command << std::endl;

	if (tokens.size() > 0 && commands.find(tokens[0]) != commands.end())
	{
		if (tokens[0] != "PASS" && clients[clientSocket]->getHasPassword() == false)
		{
			sendToClient(clientSocket, ERR_FATALERROR("You must send connect with the right password first"));
		}
		else if ((*clients[clientSocket]).getIsRegistered() == false && tokens[0] != "PASS" && tokens[0] != "NICK" && tokens[0] != "USER")
		{
			sendToClient(clientSocket, ERR_NOTREGISTERED());
		}
		else
		{
			commands[tokens[0]](*this, *clients[clientSocket], tokens);
		}
	}

}

void	Server::sendToClient(int fd, std::string const & message)
{
	if (shutting_down == true)
		return ;
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error: send failed");
	std::cout << (std::string)BLUE << "SENT TO CLIENT [" << fd - 3 << "] : " << message << NOCOLOR;
}

void	Server::disconnectClient(Client & client)
{
	int i = client.getiPoll();

	std::cout << "Client number [" << client.getFd() - 3 << "] disconnected." << std::endl;

	pollfds[i] = pollfds[nbSockets - 1]; //move last client to the empty slot
	clients[pollfds[i].fd]->setiPoll(i); //update the index of the client in the pollfds array
	

	for (it_channels it = client.getChannelsBegin(); it != client.getChannelsEnd(); it++)
	{
		it->second->removeClient(client.getFd());
	}

	for (it_channels it = channels.begin(); it != channels.end(); it++)
	{
		if (it->second->isBanned(client.getFd()) == true)
			it->second->removeBannedUser(client.getFd());
	}

	clients.erase(client.getFd());
	delete &client;
	nbSockets--;

}





std::string Server::getPassword()
{
	return password;
}

it_clients Server::getClientsBegin()
{
	return clients.begin();
}

it_clients Server:: getClientsEnd()
{
	return clients.end();
}

it_channels Server::getChannelsBegin()
{
	return channels.begin();
}

it_channels Server::getChannelsEnd()
{
	return channels.end();
}

void Server::registerDateCreation()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", timeinfo);
	creationDate = buffer;
}

std::string Server::getCreationDate()
{
	return creationDate;
}

void Server::mySigIntHandler(int s)
{
	std::cout << std::endl << "Caught signal " << s << std::endl;
	Server::running = false;
}

void Server::sendToAllClients(std::string const & message)
{
	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getIsRegistered() == true)
			sendToClient(it->second->getFd(), message);
	}
}

void Server::sendToAllClientsFromBot(std::string const & message, bool sendToWinners)
{
	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getIsRegistered() == true && it->second->getBotEnabled() == true)
		{
			if (sendToWinners == false && bot.isAWinner(it->second->getFd()) == true)
			{
				continue ;
			}
			sendToClient(it->second->getFd(), message);
		}
	}
}

void Server::sendToAllClientsInChannel(std::string const & channelName, std::string const & message)
{
	for (it_channels it = channels.begin(); it != channels.end(); it++)
	{
		if (it->first == channelName)
		{
			it->second->sendToAllClients(message);
			return ;
		}
	}
}

void Server::sendToAllClientsInChannelExceptOne(int fd, std::string const & channelName, std::string const & message)
{
	for (it_channels it = channels.begin(); it != channels.end(); it++)
	{
		if (it->first == channelName )
		{
			it->second->sendToAllClientsExceptOne(fd, message);
			return ;
		}
	}
}

void Server::addChannel(std::string const & channelName, Channel * channel)
{
	channels[channelName] = channel;
}

void Server::removeChannel(std::string const & channelName)
{
	if (channels.find(channelName) != channels.end())
		channels.erase(channelName);
}

void Server::removeClientFromChannel(std::string const & channelName, int fd)
{
	clients[fd]->removeChannel(channelName);
	channels[channelName]->removeClient(fd);
}

void Server::sendAnswerToBot(int fd, std::string const & message)
{
	bot.receiveAnswer(fd, message);
}

bool Server::noRegisteredClients()
{
	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getIsRegistered() == true)
			return false;
	}
	return true;
}

Client * Server::getClientFromFd(int fd)
{
	return clients[fd];
}

Client * Server::getClientFromNickname(std::string const & nickname)
{
	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return it->second;
	}
	return NULL;
}

std::string Server::getNbClients()
{
	return intToString(clients.size());
}

std::string Server::getNbClientsUnregistered()
{
	int nb = 0;
	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getIsRegistered() == false)
			nb++;
	}
	return intToString(nb);
}

std::string Server::getNbChannels()
{
	return intToString(channels.size());
}

std::string Server::getMaxClients()
{
	return intToString(maxClients);
}
