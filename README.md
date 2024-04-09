# 🤔 Description
This project is about creating our own IRC server. We used an actual IRC client to connect to our server and test it.
**IRC** (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers created in **1988**. It is mainly used for group discussion in chat rooms called “**channels**” although it supports private messages between two users, data transfer, and various client-side commands.
To build an IRC Server, we used sockets coupled with some basic system functions: poll, accept, recv, send.

## Mandatory part
  - We have to develop an IRC server in C++ 98.
  - The server must be capable of handling multiple clients at the same time and never hang.
  - Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
  - An IRC client must be able to connect to your server without encountering any error.
  - Communication between client and server has to be done via TCP/IP (v4).
  - Using the IRC client with our server must be similar to using it with any official IRC server:
    -  We must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using the IRC client.
    -  All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
    -  We must have operators and regular users.
    -  Operators have specific commands.

## Bonus part 
Here are the extra features we added to our IRC server so it looks even more like and actual IRC server:
  - Handle file transfer.
  - A quiz Bot, rewarding winners with points and ranking the participants.
  - Enhance the commands by adding features and options to it.

# 🔑 Our IRC Commands
These are the available commands in our IRC Server:
  - **PASS**: The **PASS** command is used to set a 'connection password'.
  - **NICK**: NICK command is used to give user a nickname or change the existing one.
  - **USER**: he USER command is used at the beginning of connection to specify the username, hostname and realname of a new user.
  - **QUIT**: A client session is terminated with a quit message.
  - **JOIN**: The JOIN command is used by a user to request to start listening to the specific channel.
  - **PART**: The PART command causes the user sending the message to be removed from the list of active members for all given channels listed in the parameter string.
  - **KICK**: The KICK command can be used to request the forced removal of a user from a channel.
  - **TOPIC**: The TOPIC command allows channel operators to update a channel's Topic.
  - **PRIVMSG**: PRIVMSG is used to send private messages between users, as well as to send messages to channels.
  - **INVITE**: INVITE allows a member of a channel to invite another member to this channel.
  - **SENDFILE**: The SENDFILE command is used to send a file to another user.
  - **GETFILE**: The GETFILE command is used to receive a file from another user.
  - **MODE**: The user MODE's are typically changes which affect either the settings of a channels.
      MODE's parameters:
          - '-o': Used by a channel operator to give/take operator privilege to/from another client.
          - '-t': Sets or unsets the TOPIC for a channel.
          - '-k': Creates or deletes the password requiered to join a channel.
          - '-l': Sets of unsets the max number of clients allowed in a channel.
          - '-i': Changes the channel to an 'invite-only- channel.

# ⚙️ Start IRC Server
To compile the program, use:
  - `make`
To start the Server, use:
  - `./ircserv <port> <password>`
    - **port**: The port number on which your IRC server will be listening to for incoming IRC connections.
    - **password**: The connection password. It will be needed by any IRC client that tries to connect to your server.
To connect to the server, you can use:
  - `nc <IP ADDRESS> <PORT>`
    - **IP ADDRESS**: Host IP address.
    - **PORT**: The PORT that the server listening on.
    - Once connected, use PASS, USER and NICK in that order to successfully login.
  - You can also use an IRC Client e.g LimeChat, WeeChat, Hexchat and others.

# 💪 Contributers
[ABDYLI Arbesa](https://github.com/aarbesa)<br/>
&emsp;42intra profile : [aabdyli](https://profile.intra.42.fr/users/aabdyli) <br/>
<br/>
[RUET SUQUET William](https://github.com/willrsq1)
<br/>
&emsp;42intra profile : [wruet-su](https://profile.intra.42.fr/users/wruet-su)
<br/>

# 👨‍💻 Ressources
* [IRC Norme](https://modern.ircdocs.horse/)

# Peer Review
This project was reviwed by three of our peers who gave us some feedback.
![image](https://github.com/willrsq1/Ft_IRC/assets/117649637/44cbe4ca-ccea-4a50-9e0e-1a578dbf0577)


Description inspired by barimehdi77
