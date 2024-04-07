#include "../Includes/Channel.hpp"

Channel::Channel(std::string name, Server * server): name(name), topic(""), server(server), mode("t"), limit(false), limitValue(0), inviteOnly(false), topicProtected(true), topicEditor(""), topicDate(""), hasKey(false), key("")
{

}

Channel::~Channel()
{
	
}

void Channel::addClient(int fd, Client * client)
{
	if (isBanned(fd))
	{
		server->sendToClient(fd, ERR_FATALERROR("You are banned from this channel."));
		return ;
	}
	clients[fd] = client;
	if (isClientInvited(fd))
	{
		removeInvitedUser(fd);
	}
	if (operators.size() == 0)
	{
		setNewOperator(fd);
		server->sendToAllClientsInChannel(this->name, MODE(SERVER, SERVER, this->name, "+o " + this->clients.begin()->second->getNickname()));

	}
}

bool Channel::isClientInChannel(int fd)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{
		
		if ((*it).second->getFd() == fd)
			return (true);

	}
	return (false);
}

std::string & Channel::getName()
{
	return (this->name);
}

void Channel::sendToAllClients(std::string const & message)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{

		server->sendToClient((*it).second->getFd(), message);

	}
}

void Channel::sendToAllClientsExceptOne(int fd, std::string const & message)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{
		
		if (fd == (*it).second->getFd())
			continue ;
		server->sendToClient((*it).second->getFd(), message);

	}
}

void Channel::removeOperator(int fd)
{
	for (std::vector<int>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{

		if (*it == fd)
		{
			this->operators.erase(it);
			return ;
		}

	}
}

bool Channel::isOperator(int fd)
{
	for (std::vector<int>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{

		if (*it == fd)
			return (true);

	}
	return (false);
}

void Channel::removeClient(int fd)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if ((*it).second->getFd() == fd)
		{
			if (this->isOperator(fd))
				this->operators.erase(std::find(this->operators.begin(), this->operators.end(), fd));
			if (this->isClientInvited(fd))
				this->invitedUsers.erase(std::find(this->invitedUsers.begin(), this->invitedUsers.end(), fd));
			this->clients.erase(it);
			break ;
		}

	}

	if (this->clients.size() == 0)
	{
		server->removeChannel(this->name);
		delete this;
		return ;
	}

	if (this->operators.size() == 0)
	{
		this->setNewOperator(this->clients.begin()->second->getFd());
		server->sendToAllClientsInChannel(this->name, MODE(SERVER, SERVER, this->name, "+o " + this->clients.begin()->second->getNickname()));

	}
}

void Channel::setNewOperator(int fd)
{
	this->operators.push_back(fd);
}

void Channel::addBannedUser(int fd)
{
	this->bannedUsers.push_back(fd);
}

void Channel::removeBannedUser(int fd)
{
	for (std::vector<int>::iterator it = bannedUsers.begin(); it != bannedUsers.end(); it++)
	{
		if (*it == fd)
		{
			bannedUsers.erase(it);
			return ;
		}
	}
}

bool Channel::isBanned(int fd)
{
	for (std::vector<int>::iterator it = bannedUsers.begin(); it != bannedUsers.end(); it++)
	{
		if (*it == fd)
			return (true);
	}
	return (false);
}

int Channel::getNbClients()
{
	return (this->clients.size());
}

std::string & Channel::getTopic()
{
	return (this->topic);
}

std::string & Channel::getMode()
{
	return (this->mode);
}

bool Channel::isSetMode(char c)
{
	for (std::string::iterator it = this->mode.begin(); it != this->mode.end(); it++)
	{
		if (*it == c)
			return (true);
	}
	return (false);
}

int Channel::getLimitValue()
{
	return (this->limitValue);
}

void Channel::setLimitValue(int limitValue)
{
	this->limitValue = limitValue;
}

void Channel::setMode(char c)
{
	if (this->isSetMode(c))
		return ;
	this->mode += c;
}

void Channel::unsetMode(char c)
{
	if (!this->isSetMode(c))
		return ;
	this->mode.erase(std::find(this->mode.begin(), this->mode.end(), c));
}

bool Channel::limitIsReached()
{
	if (this->limit == false)
		return (false);
	if (this->limitValue <= this->getNbClients())
		return (true);
	return (false);
}

void Channel::setLimit(bool limit)
{
	this->limit = limit;
}

int Channel::getNbOperators()
{
	return (this->operators.size());
}

void Channel::addInvitedUser(int fd)
{
	this->invitedUsers.push_back(fd);
}

void Channel::removeInvitedUser(int fd)
{
	for (std::vector<int>::iterator it = invitedUsers.begin(); it != invitedUsers.end(); it++)
	{
		if (*it == fd)
		{
			invitedUsers.erase(it);
			return ;
		}
	}
}

bool Channel::isClientInvited(int fd)
{
	for (std::vector<int>::iterator it = invitedUsers.begin(); it != invitedUsers.end(); it++)
	{
		if (*it == fd)
			return (true);
	}
	return (false);
}

void Channel::clearInvitedUsers()
{
	this->invitedUsers.clear();
}

void Channel::setInviteOnly(bool inviteOnly)
{
	this->inviteOnly = inviteOnly;
}

bool Channel::isInviteOnly()
{
	return (this->inviteOnly);
}

bool Channel::isTopicProtected()
{
	return (this->topicProtected);
}

void Channel::setTopicProtected(bool topicProtected)
{
	this->topicProtected = topicProtected;
}

void Channel::setTopic(std::string const & topic, std::string const & nickname)
{
	topicEditor = nickname;
	this->topic = topic;

	std::time_t timestamp = std::time(NULL);
	topicDate = intToString(timestamp);
}

std::string & Channel::getTopicEditor()
{
	return (this->topicEditor);
}

std::string & Channel::getTopicDate()
{
	std::cout << "topicDate: " << topicDate << std::endl;
	return (this->topicDate);
}

void Channel::setKey(std::string const & key)
{
	this->key = key;
	this->hasKey = true;
}

bool Channel::hasAKey()
{
	return (this->hasKey);
}

std::string & Channel::getKey()
{
	return (this->key);
}

void Channel::clearKey()
{
	this->hasKey = false;
	this->key = "";
}

it_clients Channel::getClientsBegin()
{
	return (this->clients.begin());
}

it_clients Channel::getClientsEnd()
{
	return (this->clients.end());
}