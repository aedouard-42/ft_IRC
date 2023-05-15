/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:06:57 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/04 01:42:55 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

 Server::Server()
{
}

Server::Server(int port, std::string pwd, bool file_output)
:
_file_output(file_output),
_server_pwd(pwd)
{
	_server_pwd = serverhash(pwd);
	init_socket(port);
	_clientSockets.push_back(this->createPollfd(_entrySocket));

	_servercommands.insert(std::make_pair("NOTICE", new Notice(this)));
	_servercommands.insert(std::make_pair("NICK", new Nick(this)));
	_servercommands.insert(std::make_pair("OPER", new Oper(this)));
	_servercommands.insert(std::make_pair("HELP", new Help(this)));
	_servercommands.insert(std::make_pair("JOIN", new Join(this)));
	_servercommands.insert(std::make_pair("LIST", new List(this)));
	_servercommands.insert(std::make_pair("USER", new Usercmd(this)));
	_servercommands.insert(std::make_pair("QUIT", new Quit(this)));
	_servercommands.insert(std::make_pair("PING", new Ping(this)));
	_servercommands.insert(std::make_pair("PRIVMSG", new PrivMsg(this)));
	_servercommands.insert(std::make_pair("PASS", new Pass(this)));
	_servercommands.insert(std::make_pair("TOPIC", new Topic(this)));
	_servercommands.insert(std::make_pair("PART", new Part(this)));
	_servercommands.insert(std::make_pair("NAMES", new Names(this)));

    _opcommands.insert(std::make_pair("BAN", new ChannelBan(this)));
	_opcommands.insert(std::make_pair("KICK", new Kick(this)));
    /* TO DO
    KILL <client> <comment>
    DIE (command to shutdown server)
    */


	/**************************************************************************/
	_passop = "mpm";
	Client tmp(this, "Server_Machine_Admin", "SM_Admin", "SM_Admin");
	_clientSockets.push_back(createPollfd(0));
	tmp.changeName(std::string("ServerAdmin"));
	tmp.addBasicCommands();
	tmp.becomeOperator();
	addClient(tmp, 0);
	/**************************************************************************/
}



/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
 
Server::~Server()
{
    serverLogMssg("Server shut down");
}



/*
** --------------------------------- PUBLIC METHODS ----------------------------
*/

//	main server routine, accepting client  and preccesing requests
void							Server::routine()
{
	while (1)
	{
		this->pollRoutine();
	}
}

//	sending message (*mssg* std::string) to a specific (*sendTo* client), adding prefix and \r\n
void	Server::sendToClient(Client const &sendTo, std::string prefix, std::string mssg)
{
	std::string str = prefix + std::string(" ") + mssg + std::string("\r\n");
	send(sendTo.getFd(), (str).c_str(), str.length(), 0);
	str = "";
	std::stringstream ss;  
  	ss << sendTo.getFd();  
  	ss >> str;
	serverLogMssg(std::string
			(
				"message sent to <"
				+ sendTo.getNname()
				+ std::string("> fd ")
				+ str
				+ std::string("|")
				+ prefix 
				+ std::string(" ")
				+ mssg 
				+ "|\r\n"
			));
}

//	sending message (*mssg* std::string) to a specific (*sendTo* client), adding prefixed server name and \r\n
void	Server::sendToClient(Client const &sendTo, std::string mssg)
{
	sendToClient(sendTo, std::string(SERVER_NAME), mssg);
}

// adding new channel to existing one's
Server::channelmap::iterator	Server::addChannel(Channel &newchan)
{
	return _channels.insert(std::make_pair(newchan.getName(), newchan)).first;
}


//	adding client, using recv -> parsing user info -> adding new user | sending an error mssg
void	Server::addClient(Client &user, int fd)
{
	_usersMap.insert(std::make_pair(fd, user));
	serverLogMssg(std::string("new client <" + user.getNname() + " > added to the client list"));
}

//	add a client in the "pending list", for clients who doesn't send right PASS NICK and USER
void	Server::addPendingClient(Client &pendingUser, int fd)
{
	_clientSockets.push_back(createPollfd(fd));
	_pendingClients.insert(std::make_pair(fd, pendingUser));
	serverLogMssg("Added new pending user");
}

// deleting client
void	Server::deleteClient(std::string nname)
{
	std::map<int, Client>::iterator it = _usersMap.begin();
	std::map<int, Client>::iterator ite = _usersMap.end();
	while (it != ite)
	{
		if (it->second.getNname() == nname)
		{
			deleteFdfPoll(it->first);
			_usersMap.erase(it);
			break;
		}
		it++;
	}
}



//	free everything, close sockets and exit the server
void	Server::closeServer()
{
	// Clearing all pollFd
	std::vector<struct pollfd>::iterator clientSocketsIterator = _clientSockets.begin();
	while (clientSocketsIterator != _clientSockets.end())
	{
		close (clientSocketsIterator->fd);
		clientSocketsIterator++;
	}
	_clientSockets.clear();
	_clientSockets.reserve(0);
	*logStream << "client socket size = " << _clientSockets.size() << std::endl;
	*logStream << "client socket capasity = " << _clientSockets.size() << std::endl;

	// Clearing all Channels
	_channels.clear();

	// Clearing all Users
	_usersMap.clear();
	_pendingClients.clear();

	{
		// Clearing all Serv.Commmands
		std::map<std::string, Command *>::iterator commandMapIterator = _servercommands.begin();
		while (commandMapIterator != _servercommands.end())
		{
			delete(commandMapIterator->second);
			commandMapIterator++;
		}
		_servercommands.clear();
	}

	{
		// Clearing all Operator Commands
		std::map<std::string, Command *>::iterator commandMapIterator = _opcommands.begin();
		while (commandMapIterator != _opcommands.end())
		{
			delete(commandMapIterator->second);
			commandMapIterator++;
		}
		_opcommands.clear();
	}

	if (_file_output == true)
	{
		((std::ofstream*)(logStream))->close();
		delete (logStream);
	}

	exit(0);
}



// checking if the password to entry is correct
bool			Server::checkOpPass(std::string pass) const {
	if (serverhash(pass) == _passop)
		return (true);
	return (false);
}


bool	Server::checkServerPass(std::string pass) const {
	if (serverhash(pass) == _server_pwd)
		return (true);
	return (false);
}





// search a client int the corresponding map, by his fd
Client *	Server::searchClient(int fd, clientmap * map) 
{
	clientmap::iterator it = map->find(fd);
	if (it == map->end())
		return NULL;
	return (&(it->second));
}



Client	*	Server::searchClient(std::string nickName) 
{
	std::map<int, Client>::iterator it = _usersMap.begin();
	std::map<int, Client>::iterator ite = _usersMap.end();
	while (it != ite)
	{
		if (it->second.getNname() == nickName)
			return (&(it->second));
		it++;
	}
	return (NULL);
}


// searching channel by *channame*, returning a pointer to the found channel, NULL if not found
Channel *Server::searchChannel(std::string channame) 
 {
    channelmap::iterator it = _channels.find(channame);
    if (it == _channels.end())
        return (NULL);
    return (&(it->second));
}







/*
** --------------------------------- PRIVATE METHODS ---------------------------
*/

void Server::init_socket(int port)
{
	struct sockaddr_in _address;

    _entrySocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    serverLogMssg("socket created");	
    
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    _address.sin_port = htons(port);

    if ((bind(_entrySocket, (struct sockaddr *)&_address, sizeof(_address))) != 0)
    {
        serverLogMssg("socket bind failure");
        return ;
    }
    else
        serverLogMssg("socket bind succeded");

    if ((listen(_entrySocket, 1024)) != 0)
    {
        serverLogMssg("listen() failure");
        return ;
    }
    else
        serverLogMssg("server is listening");
    return ;
}



// main poll routine, checking and proccessing events if any occurs
void	Server::pollRoutine()
{
	if (poll(&(*(_clientSockets.begin())), _clientSockets.size(), 500) > 0)
	{
		serverLogMssg(" pool detected something:");
		for (unsigned int i = 0; i < _clientSockets.size(); i++)
		{
			if (_clientSockets[i].revents != 0)
				this->proccessEventFd(i);
		}
	}
}



// proccesing events if any event occured on a socket
void	Server::proccessEventFd(int i)
{
	char buff[552];
	*logStream << "\t_clientSockets[" << i << "] had a revent, fd = " << _clientSockets[i].fd << std::endl;
	if (_clientSockets[i].fd == 0)
		this->executeMachCmds(buff);
	else if (_clientSockets[i].fd == _entrySocket)
		this->acceptClient();
	else
	{
		Client * tmp = searchClient(_clientSockets[i].fd, &_pendingClients);
		if (tmp) {
			serverLogMssg("entering ProccessPendingClient");
			proccessPendingClient(tmp);
		}
		else {
			tmp = searchClient(_clientSockets[i].fd, &_usersMap);
			if (tmp) {
				serverLogMssg("entering ProccessRegisteredClient");
				proccessRegisteredClient(tmp);
			}
			else
			{
				serverLogMssg("deleting a Fd from poll");
				deleteFdfPoll(_clientSockets[i].fd);
			}
		}
		
	}
	_clientSockets[i].revents = 0;
}






// executin commands typed on the server terminal, serv.admin only
void	Server::executeMachCmds(char * buff)
{
	buff[read(0, buff, 552)] = 0;
	*logStream << "\treceived mssg = " << buff << std::endl;
	std::string tmp(buff);
	if (tmp.compare("exit\n") == 0)
		this->closeServer();
	else if (tmp.compare("list users\n") == 0)
	{
		*logStream << "LIST OF USERS:" << std::endl;
		for (clientmap::iterator it = _usersMap.begin(); it != _usersMap.end(); it++)
			*logStream << "fd <" << it->first << "> user <" << it->second.getNname() << std::endl;
		*logStream << std::endl;
	}
	else if (tmp.compare("channel list\n") == 0)
	{
		*logStream << "LIST OF CHANNELS:" << std::endl;
		for (channelmap::iterator it = _channels.begin(); it != _channels.end(); it++)
			*logStream << "channel <" << it->first << "> with " << it->second.getClients().size() << " users" << std::endl;
		*logStream << std::endl;

	}
	else if (tmp.compare("create channel\n") == 0)
	{
		std::string newChannelName;
		std::cout << "Type the new channel name, then press ENTER" << std::endl;
		std::cin >> newChannelName;

		Channel toadd = Channel(this, newChannelName);
		addChannel(toadd);
	} else {
		Client &admin = _usersMap.find(0)->second;
		parseClientSent(buff, admin);
	}
}


// accepting new client, by verifying if the NickName isn't in use already
void	Server::acceptClient()
{
	struct sockaddr_in _client;

    socklen_t len = sizeof(_client);
    int	client_fd = -1;
	
	client_fd = accept(_entrySocket, (struct sockaddr *)&_client, &len);
 	if (client_fd >= 0) {
		Client tmp = Client(this, "pending_user", client_fd);
		addPendingClient(tmp, client_fd);
	}
	else
		serverLogMssg("accept failure");
}


void	Server::proccessPendingClient(Client * pendingClient)
{
	char buff[553];
	int rcvRet;

	rcvRet = recv(pendingClient->getFd(), (void*) buff, 552, 0);
	if (rcvRet > 0)
	{
		if (rcvRet > 552)
			buff[552] = 0;
		else
			buff[rcvRet] = 0;
		
		*logStream << "\treceived mssg = " << buff << std::endl;
		*logStream << "PROCESSING PENDING CLIENT:" << std::endl;

		std::vector<std::string> msgs = ftirc_split(buff, "\r\n");

		std::vector<std::string>::iterator msgsit = msgs.begin();
		std::vector<std::string>::iterator msgsite = msgs.end();
		
		while( msgsit != msgsite)
		{
			size_t first_space = msgsit->find(' ');
			std::string cmdName = msgsit->substr(0, first_space);
			if (first_space == std::string::npos)
				first_space = msgsit->size();
			std::string cmdArgs = msgsit->substr((first_space == msgsit->size() ? first_space : first_space + 1));
			*logStream << "str = |" << cmdName << "' '" << cmdArgs << "|" << std::endl;
			if (pendingClient->getPassStatus() == false)
			{
				if (cmdName == "PASS")
				{
					pendingClient->execute(cmdName, cmdArgs);
				}
			}
			else
			{
				if (cmdName == "NICK" || cmdName == "USER")
				{
					pendingClient->execute(cmdName, cmdArgs);
				}
			}
			msgsit++;
		}
	}
	else if (rcvRet == 0)
	{
		*logStream << "One of Pending client's disconected, closing the corresponding socket" << std::endl;
		deleteFdfPoll(pendingClient->getFd());
		_pendingClients.erase(pendingClient->getFd());
		return ;
	}	
	if (!pendingClient->isPending())
	{
		pendingClient->getLoggedOn();
		_usersMap.insert(std::make_pair(pendingClient->getFd(), *pendingClient));
		serverLogMssg(std::string("new client <" + pendingClient->getNname() + " > added to the client list"));
		_pendingClients.erase(pendingClient->getFd());
	}
}

void	Server::proccessRegisteredClient(Client * client)
{
	char buff[553];
	int rcvRet;

	rcvRet = recv(client->getFd(), (void*) buff, 552, 0);
	if (rcvRet > 0)
	{
		if (rcvRet > 552)
			buff[552] = 0;
		else
			buff[rcvRet] = 0;

		*logStream << "\treceived mssg = " << buff << std::endl;

		std::vector<std::string> msgs = ftirc_split(buff, "\r\n");

		std::vector<std::string>::iterator msgsit = msgs.begin();
		std::vector<std::string>::iterator msgsite = msgs.end();
		
		while( msgsit != msgsite)
		{
			size_t first_space = msgsit->find(' ');
			std::string cmdName = msgsit->substr(0, first_space);
			if (first_space == std::string::npos)
				first_space = msgsit->size();
			std::string cmdArgs = msgsit->substr((first_space == msgsit->size() ? first_space : first_space + 1));
			client->execute(cmdName, cmdArgs);
			msgsit++;
		}
		
	}
	else if (rcvRet == 0)
	{
		*logStream << "One of Registered client's disconected, closing the corresponding socket" << std::endl;
		deleteClient(client->getNname());
	}

}

bool	Server::parseClientSent(char * buff, Client &user) 
{
	std::vector<std::string> msgs = ftirc_split(buff, "\r\n");
	std::vector<std::string>::iterator msgit = msgs.begin();

	size_t pos;
	try {
		while (msgit != msgs.end()) {
			pos = msgit->find(' ');
			if (pos == std::string::npos)
				pos = msgit->size();
			if (!user.execute(msgit->substr(0, pos), msgit->substr((pos == msgit->size() ? pos : pos + 1))))
				clientLogMssg(*msgit);
			++msgit;
		}
	} catch (Nick::NameTakenException &nte) {
		sendToClient(user, nte.what());
		return (false);
	}
	return (true);
}






//	returning serverhash
std::string		&Server::serverhash(std::string &toHash) const {
	std::string ret = "";
	for(std::string::size_type i = 0; i < toHash.size(); ++i) {
    	ret += toHash[i] + 1;
		// si plus compliqué nécessaire : (toHash[i] + i) % 126 + 1;
	}
	//Debugging
	serverLogMssg("Hashed : " + toHash + " into " + ret);
	toHash = ret;
	return (toHash);
}





struct pollfd	Server::createPollfd(int fd)
{
	struct pollfd tmp;
	tmp.fd = fd;
	tmp.events = POLLIN;
	tmp.revents = 0;
	return (tmp);
}

void			Server::deleteFdfPoll(int fd)
{
	std::vector<struct pollfd>::iterator it = _clientSockets.begin();
	std::vector<struct pollfd>::iterator ite = _clientSockets.end();

	close(fd);
	while (it != ite)
	{
		if (it->fd == fd)
		{
			_clientSockets.erase(it);
			break;
		}
		it++;
	}
}

// std::string Server::send_reply(int code, std::string arg1 = std::string(""), std::string arg2 = std::string(""), std::string arg3 = std::string("") , std::string arg4 = std::string(""))
// {
//     std::stringstream str_stream;
//     str_stream << code;
//     std::string code_str;
//     str_stream >> code_str;
//     while (code_str.length() <  3)
//     {
//         code_str = "0" + code_str;
//     }
//     //std::cout << code_str << std::endl;
//     std::string nick = this->getNname();
//     std::string reply = get_reply(code , arg1, arg2, arg3, arg4);

//     return code_str + nick + reply;
// }



/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const Server::commandmap &Server::getServCommands() const {
    return (_servercommands);
}

const Server::commandmap &Server::getOpCommands() const {
    return (_opcommands);
}

const Server::channelmap &Server::getChannels() const {
    return(_channels);
}

const Server::clientmap &Server::getClients() const {
	return(_usersMap);
}

/* ************************************************************************** */