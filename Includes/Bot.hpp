#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <map>
# include <vector>

class Server;

class Bot
{
	private:

		time_t lastUpdate;
		int mode;
		int nbWinners;
		std::vector<int> winners;
		Server * server;

		int questionNb;
		int questionsTotalNb;

		std::map<int, std::string> questions;
		std::map<int, std::string> answers;

		std::string getWinners();

	public:
		Bot();
		~Bot();

		void update();
		void setServer(Server * server);

		void receiveAnswer(int fd, std::string const & answer);
		bool isAWinner(int fd);
};

#include "Server.hpp"

#endif