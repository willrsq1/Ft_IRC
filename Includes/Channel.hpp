#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <map>
#include <iostream>
#include <vector>

class Client;
class Server;

typedef	std::map<int, Client *>::iterator 	it_clients;

class Channel
{
	private:

		std::map<int, Client *> clients;
		std::vector<int> operators;
		std::vector<int> bannedUsers;
		std::vector<int> invitedUsers;
		std::string name;
		std::string topic;
		Server * server;
		std::string mode;

		bool limit;
		int limitValue;
		bool inviteOnly;
		bool topicProtected;

		std::string topicEditor;
		std::string topicDate;

		bool	hasKey;
		std::string key;


		Channel();

	public:
		Channel(std::string name, Server * server);
		~Channel();

		std::string & getName();



		//OPERATORS
		void removeOperator(int fd);
		bool isOperator(int fd);
		void setNewOperator(int fd);
		int getNbOperators();



		bool isClientInChannel(int fd);
		void sendToAllClients(std::string const & message);
		void sendToAllClientsExceptOne(int fd, std::string const & message);
		void addClient(int fd, Client * client);
		void removeClient(int fd);

		int getNbClients();
		std::string & getTopic();


		void addBannedUser(int fd);
		void removeBannedUser(int fd);
		bool isBanned(int fd);


		std::string & getMode();
		bool isSetMode(char c);
		void setMode(char c);
		void unsetMode(char c);
		void setLimitValue(int limitValue);
		int getLimitValue();
		bool limitIsReached();
		void setLimit(bool limit);


		void addInvitedUser(int fd);
		void removeInvitedUser(int fd);
		bool isClientInvited(int fd);

		void setInviteOnly(bool inviteOnly);
		bool isInviteOnly();
		void clearInvitedUsers();

		void setTopicProtected(bool topicProtected);
		bool isTopicProtected();
		void setTopic(std::string const & topic, std::string const & nickname);
		std::string & getTopicEditor();
		std::string & getTopicDate();

		void setKey(std::string const & key);
		std::string & getKey();
		bool hasAKey();
		void clearKey();

		it_clients getClientsBegin();
		it_clients getClientsEnd();
};

#include "Client.hpp"
#include "Server.hpp"
#endif