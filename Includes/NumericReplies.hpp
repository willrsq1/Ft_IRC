#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

# define BLUE        "\033[1;34m"
# define NOCOLOR     "\033[0m"

//x

#define RPL_NEWNICKNAME(client, username, new_nickname) ((std::string)":" + SRC_CLI(client, username) + " NICK " + new_nickname + "\r\n")

//x

#define ERR_FATALERROR(msg) ((std::string)":" + SERVER + " ERROR " + msg + "\r\n")

#define SRC_CLI(client, username) ((std::string)client + "!" + username + "@" + HOST)

#define PRIVMSG(client, username, target, message) ((std::string)":" + SRC_CLI(client, username) + " PRIVMSG " + target + " :" + message + "\r\n")

#define JOIN(client, username, channel) ((std::string)":" + SRC_CLI(client, username) + " JOIN " + channel + "\r\n")

#define PART(client, username, target, reason) ((std::string)":" + SRC_CLI(client, username) + " PART " + target +  " " + reason + "\r\n")

#define KICK(client, username, channel, target, reason) ((std::string)":" + SRC_CLI(client, username) + " KICK " + channel + " " + target + " :" + reason + "\r\n")

#define PONG(client, token) ((std::string)":" + SERVER + " PONG " + client + " " + token + "\r\n")

#define QUIT(client, username, reason) ((std::string)":" + SRC_CLI(client, username) + " QUIT " + client + " :Quit: " + reason + "\r\n")

#define INVITE(client, username, target, channel) ((std::string)":" + SRC_CLI(client, username) + " INVITE " + target + " " + channel + "\r\n")

#define TOPIC(client, username, channel, topic) ((std::string)":" + SRC_CLI(client, username) + " TOPIC " + channel + " " + topic + "\r\n")

#define MODE(client, username, channel, mode) ((std::string)":" + SRC_CLI(client, username) + " MODE " + channel + " " + mode + "\r\n")

//001
#define RPL_WELCOME(client, username) ((std::string)":" + SERVER + " 001 " + client + " :Welcome to the " + SERVER + " Network, " + SRC_CLI(client, username) + "\r\n")
//002
#define RPL_YOURHOST(client) ((std::string)":" + SERVER + " 002 " + client + " :Your host is " + SERVER + ", running version 1.0\r\n")
//003
#define RPL_CREATED(client, datetime) ((std::string)":" + SERVER + " 003 " + client + " :This server was created " + datetime + "\r\n")
//004
#define RPL_MYINFO(client) ((std::string)":" + SERVER + " 004 " + client + " " + SERVER + " version 1.0 " + USERMODE + " " + CHANMODE + " " + CHANMODES_WITH_PARAMS + "\r\n")
//005
#define RPL_ISUPPORT(client) ((std::string)":" + SERVER + " 005 " + client + " PREFIX=@ CHANTYPES=# CASEMAPPIN=rfc1459 LINELEN=512 CHANMODES=itkol CHANLIMITS=10 NICKLEN=20 USERLEN=20 TOPICLEN=50 MODES=5 NETWORK=" + SERVER + " CASEMAPPING=ascii :are supported by this server\r\n")

//221
#define RPL_UMODEIS(client, mode) ((std::string)":" + SERVER + " 221 " + client + " " + mode + "\r\n")

//251
#define RPL_LUSERCLIENT(client, nb_users) ((std::string)":" + SERVER + " 251 " + client + " :There are " + nb_users + " users and 0 invisibles and 1 server\r\n")
//252
#define RPL_LUSEROP(client) ((std::string)":" + SERVER + " 252 " + client + " 0 :operator(s) online\r\n")
//253
#define RPL_LUSERUNKNOWN(client, unregistered) ((std::string)":" + SERVER + " 253 " + client + " " + unregistered + " :unknown connection(s)\r\n")
//254
#define RPL_LUSERCHANNELS(client, nb_channels) ((std::string)":" + SERVER + " 254 " + client + " " + nb_channels + " :channels formed\r\n")
//255
#define RPL_LUSERME(client, nb_clients) ((std::string)":" + SERVER + " 255 " + client + " :I have " + nb_clients + " clients and 1 server\r\n")
//265
#define RPL_LOCALUSERS(client, nb_users, max_users) ((std::string)":" + SERVER + " 265 " + client + " " + nb_users + " " + max_users + " :Current local users " + nb_users + ", max " + max_users + "\r\n")
//266
#define RPL_GLOBALUSERS(client, nb_users, max_users) ((std::string)":" + SERVER + " 266 " + client + " " + nb_users + " " + max_users + " :Current global users " + nb_users + ", max " + max_users + "\r\n")

//321
#define RPL_LISTSTART(client) ((std::string)":" + SERVER + " 321 " + client + " Channel :Users Name\r\n")
//322
#define RPL_LIST(client, channel, nb_members, topic) ((std::string)":" + SERVER + " 322 " + client + " " + channel + " " + nb_members + " :" + topic + "\r\n")
//323
#define RPL_LISTEND(client) ((std::string)":" + SERVER + " 323 " + client + " :End of LIST\r\n")
//324
#define RPL_CHANNELMODEIS(client, channel, mode) ((std::string)":" + SERVER + " 324 " + client + " " + channel + " " + mode + "\r\n")

//331
#define RPL_NOTOPIC(client, channel) ((std::string)":" + SERVER + " 331 " + client + " " + channel + " :No topic is set\r\n")
//332
#define RPL_TOPIC(client, channel, topic) ((std::string)":" + SERVER + " 332 " + client + " " + channel + " :" + topic + "\r\n")
//333
#define RPL_TOPICWHOTIME(client, channel, nickname, datetime) ((std::string)":" + SERVER + " 333 " + client + " " + channel + " " + nickname + " " + datetime + "\r\n")

//341
#define RPL_INVITING(client, target, channel) ((std::string)":" + SERVER + " 341 " + client + " " + target + " " + channel + "\r\n")

//353
#define RPL_NAMREPLY(client, channel, prefix, users) ((std::string)":" + SERVER + " 353 " + client + " = " + channel + " :" + prefix + users + "\r\n")
//366
#define RPL_ENDOFNAMES(client, channel) ((std::string)":" + SERVER + " 366 " + client + " " + channel + " :End of NAMES list\r\n")

//372
#define RPL_MOTD(client, line) ((std::string)":" + SERVER + " 372 " + client + " :- " + line + "\r\n")
//375
#define RPL_MOTDSTART(client) ((std::string)":" + SERVER + " 375 " + client + " :- " + SERVER + " Message of the day - \r\n")
//376
#define RPL_ENDOFMOTD(client) ((std::string)":" + SERVER + " 376 " + client + " :End of MOTD command \r\n")



//401
#define ERR_NOSUCHNICK(client, target) ((std::string)":" + SERVER + " 401 " + client + " " + target + " :No such nick/channel\r\n")
//402
#define ERR_NOSUCHSERVER(client) ((std::string)":" + SERVER + " 402 " + client + " " + SERVER + " :No such server\r\n")
//403
#define ERR_NOSUCHCHANNEL(client, channel) ((std::string)":" + SERVER + " 403 " + client + " " + channel + " :No such channel\r\n")
//404
#define ERR_CANNOTSENDTOCHAN(client, channel) ((std::string)":" + SERVER + " 404 " + client + " " + channel + " :Cannot send to channel\r\n")
//405
#define ERR_TOOMANYCHANNELS(client, channel) ((std::string)":" + SERVER + " 405 " + client + " " + channel + " :You have joined too many channels\r\n")

//409
#define ERR_NOORIGIN(client) ((std::string)":" + SERVER + " 409 " + client + " :No origin specified\r\n")

//411
#define ERR_NORECIPIENT(client, command) ((std::string)":" + SERVER + " 411 " + client + " :No recipient given " + command + "\r\n")
//412
#define ERR_NOTEXTTOSEND(client) ((std::string)":" + SERVER + " 412 " + client + " :No text to send\r\n")


//417
#define ERR_INPUTTOOLONG(client) ((std::string)":" + SERVER + " 417 " + client + " :Input line was too long\r\n")

//NICK
//431
#define ERR_NONICKNAMEGIVEN(client) ((std::string)":" + SERVER + " 431 " + client + " :No nickname given\r\n")
//432
#define ERR_ERRONEUSNICKNAME(client, nickname, msg) ((std::string)":" + SERVER + " 432 " + client + " " + nickname + " :" + msg + "\r\n")
//433
#define ERR_NICKNAMEINUSE(client, nickname) ((std::string)":" + SERVER + " 433 " + client + " " + nickname + " :Nickname is already in use\r\n")

//441
#define ERR_USERNOTINCHANNEL(client, target, channel) ((std::string)":" + SERVER + " 441 " + client + " " + target + " " + channel + " :They aren't on that channel\r\n")
//442
#define ERR_NOTONCHANNEL(client, channel) ((std::string)":" + SERVER + " 442 " + client + " " + channel + " :You're not on that channel\r\n")
//443
#define ERR_USERONCHANNEL(client, target, channel) ((std::string)":" + SERVER + " 443 " + client + " " + target + " " + channel + " :is already on channel\r\n")

//451
#define ERR_NOTREGISTERED(client) ((std::string)":" + SERVER + " 451 " + client + " :You have not registered\r\n")

//PASS
//461
#define ERR_NEEDMOREPARAMS(client, command) ((std::string)":" + SERVER + " 461 " + client + " " + command + " :Wrong number of parameters\r\n")
//462
#define ERR_ALREADYREGISTRED(client) ((std::string)":" + SERVER + " 462 " + client + " :You may not reregister\r\n")
//464
#define ERR_PASSWDMISMATCH(client) ((std::string)":" + SERVER + " 464 " + client + " :Password incorrect\r\n")


//471
#define ERR_CHANNELISFULL(client, channel) ((std::string)":" + SERVER + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
//472
#define ERR_UNKNOWNMODE(client, mode) ((std::string)":" + SERVER + " 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
//472
#define ERR_INVITEONLYCHAN(client, channel) ((std::string)":" + SERVER + " 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
//475
#define ERR_BADCHANNELKEY(client, channel) ((std::string)":" + SERVER + " 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")


//482
#define ERR_CHANOPRIVSNEEDED(client, modechar) ((std::string)":" + SERVER + " 482 " + client + " " + modechar + " :You're not channel operator\r\n")

//525
#define ERR_INVALIDKEY(client, channel) ((std::string)":" + SERVER + " 525 " + client + " " + channel + " :Key is not well-formed\r\n")

//696
#define ERR_INVALIDMODEPARAM(client, target, modechar, parameter, description) ((std::string)":" + SERVER + " 696 " + client + " " + target + " " + modechar + " " + parameter + " " +  description + "\r\n")

#endif