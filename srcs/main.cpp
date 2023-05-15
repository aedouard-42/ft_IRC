/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 21:28:48 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/03 22:01:29 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "Client.hpp"

std::ostream * logStream;

std::ostream *	set_log_file(short key)
{
	if (key == STD_OUTPUT)
	{
		std::cout << "#####		LOGS OUTPUTED IN STANDART OUTPUT		#####" << std::endl << std::endl;
		return (&(std::cout));
	}
	
	std::ostream *outFile = new std::ofstream();
	((std::ofstream*)(outFile))->open(LOG_FILE, std::ofstream::out);
	*outFile << "#####		LOGS OUTPUTED IN A LOG FILE				#####" << std::endl << std::endl;
	
	return (outFile);
}

int main(int ac, char **av)
{
	/**************************************************************************/

	bool file_output;
	int port;
	std::string pwd;
	
	if (ac != 3 && ac != 4)
	{
		std::cout << "usage : ./server [PORT] [PWD]"  << std::endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (ac == 4)
		{
			logStream = set_log_file(FILE_OUTPUT);
			file_output = true;
		}
		else
		{
			logStream = set_log_file(STD_OUTPUT);
			file_output = false;
		}
		port = atoi(av[1]);
		pwd = av[2];
	}

	/***************************************************************************/
	
	Server irc(port, pwd, file_output);
	irc.routine();
	
	/*
	std::vector<std::string> truc = ftirc_split(":test :trc:lol  make::check ", " :");
	std::vector<std::string>::iterator it = truc.begin();
	while (it != truc.end()) {
		serverLogMssg(*it);
		++it;
	}
	Channel chan(&irc, "chan1");
	Channel *ret;
	Client client(&irc, "Client1");

	irc.addChannel(chan);
	ret = irc.searchChannel("chan1");
	chan.addClient(&client);
	std::cout << chan.searchClient("Client1") << std::endl;
	std::cout << chan.addToBanList(&client) << std::endl;
	std::cout << chan.searchBanned("Client1") << std::endl;
	chan.kickFromChannel(&client);
	std::cout << chan.searchClient("Client1") << std::endl;
	*logStream << ret << std::endl;
	chan.addClient(&client);
	*/


	/***************************************************************************/
	/***************************************************************************/
}