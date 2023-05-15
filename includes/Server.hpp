/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:47:37 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/04 01:12:05 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "commands/Notice.hpp"
#include "commands/Nick.hpp"
#include "commands/Topic.hpp"
#include "commands/Help.hpp"
#include "commands/Oper.hpp"
#include "commands/Join.hpp"
#include "commands/List.hpp"
#include "commands/Ping.hpp"
#include "commands/Kick.hpp"
#include "commands/Quit.hpp"
#include "commands/Pass.hpp"
#include "commands/Names.hpp"
#include "commands/Usercmd.hpp"
#include "commands/PrivMsg.hpp"
#include "commands/ChannelBan.hpp"
#include "commands/Part.hpp"


class Server
{
	public:

		typedef std::map<std::string, Command *>	commandmap;
		typedef std::map<std::string, Channel>		channelmap;
		typedef std::map<int, Client>				clientmap;
	
	private:

		int _entrySocket;

		bool								_file_output;

		std::vector<struct pollfd> 			_clientSockets;
		
		std::map<int, Client> 				_usersMap;
		std::map<int, Client> 				_pendingClients;
		
		std::map<std::string, Command *>	_servercommands;
		std::map<std::string, Command *>	_opcommands;
		std::map<std::string, Channel>		_channels;
		
		std::string							_server_pwd;
		std::string							_passop;


		void			init_socket(int port);

		void			pollRoutine();

		void			executeMachCmds(char * buff);
		void			acceptClient();
		void			proccessEventFd(int i);

		void			proccessRegisteredClient(Client * client);
		void			proccessPendingClient(Client * pendingClient);

		void			addPendingClient(Client &pendingUser, int fd);
		bool			parseClientSent(char * buff, Client &user);

		std::string		&serverhash(std::string &toHash) const;

		struct pollfd	createPollfd(int fd);
		void			deleteFdfPoll(int fd);


	class UserNotFound: public std::exception
	{
		public:
		virtual const char * what() const throw() {
			return "User not found";
		}
	};


	public:

		Server();
		Server(int port, std::string pwd, bool file_output);
		~Server();

		void					routine();
		void					sendToClient(Client const &sendTo, std::string prefix, std::string mssg);
		void					sendToClient(Client const &sendTo, std::string mssg);

		channelmap::iterator	addChannel(Channel &newchan);

		void					addClient(Client &user, int fd);
		void					deleteClient(std::string uname);

		void					closeServer();

		bool					checkOpPass(std::string pass) const;
		bool					checkServerPass(std::string pass) const;

		Client					*searchClient(std::string nickName);
		Client					*searchClient(int fd, clientmap * map);

		Channel					 *searchChannel(std::string channame);

		const commandmap	 	&getServCommands() const;
		const commandmap		&getOpCommands() const;
		const channelmap	 	&getChannels() const;
		const clientmap			&getClients() const;
};

#endif
