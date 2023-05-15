/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:32 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/03 23:07:24 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void        Client::addLoginCommands() {
    Server::commandmap smap = _serv->getServCommands();
    Server::commandmap::const_iterator it;
    std::string tab[5] = {"PASS", "USER", "NICK", "QUIT","HELP"};
    for (int i = 0; i < 5; ++i)
    {
        it = smap.find(tab[i]);
        if (it != smap.end())
            _commands.insert(std::make_pair(tab[i], it->second));        
    }
}

void        Client::addBasicCommands() {
    Server::commandmap::const_iterator it = _serv->getServCommands().begin();
    Server::commandmap::const_iterator ite = _serv->getServCommands().end();
    while (it != ite) {
        _commands.insert(std::make_pair(it->first, it->second));
        ++it;
    }
}

void        Client::addOpCommands() {
    Server::commandmap::const_iterator it = _serv->getOpCommands().begin();
    Server::commandmap::const_iterator ite = _serv->getOpCommands().end();
    while (it != ite) {
        _commands.insert(std::make_pair(it->first, it->second));
        ++it;
    }
}

Client::Client(): _username("non-spec")
{
    clientLogMssg(std::string("Client " + _username + " created"));
    _currentChannel = NULL;
    addLoginCommands();
}

Client::Client(Server *current, std::string uname) :
    _serv(current), _username(uname), _currentChannel(NULL), _pwdPass(false), _nick(false), _user(false)
{
    clientLogMssg(std::string("Client " + _username + " created"));
    addLoginCommands();

}

Client::Client(Server *current, std::string uname, int fd) :
	_serv(current), _username(uname), _fd(fd), _currentChannel(NULL), _pwdPass(false), _nick(false), _user(false)
{
	clientLogMssg(std::string("Client " + _username + " created"));
    addLoginCommands();
}

Client::Client(Server *current, std::string uname, std::string hname, std::string sname) :
    _username(uname), _hostname(hname), _servername(sname), _pwdPass(false), _nick(false), _user(false)
{
    _serv = current;
    clientLogMssg(std::string("Client " + _username + " created"));
    _currentChannel = NULL;
    addLoginCommands();

}

Client::Client(Client const & var)
{
	_serv = var.getServer();
	_username = var.getUname();
	_nickname = var.getNname();
	_hostname = var.getHname();
	_servername = var.getSname();
	_realname = var.getRname();
	_fd = var.getFd();
	_currentChannel = var.getChannel();
	_pwdPass = var.getPassStatus();
	_nick = var.getNickstatus();
	_user = var.getUserStatus();
    _commands = var.getCommands();
}

Client::~Client()
{
    clientLogMssg(" Client " + _username + " destroyed");
}

void	    Client::becomeOperator() {
    this->addOpCommands();
}

bool	    Client::execute(std::string const &cmd, std::string const &restline) {
    commandmap::iterator cit = _commands.find(cmd);
    if (cit == _commands.end()) {
        clientLogMssg("Command '" + cmd + "' not found in available commands for the client : " + _username);
        receive_reply(421, cmd);
        return (false);
    }
    clientLogMssg("Executing " + cmd + " command.");
    cit->second->execute(restline, *this);
    clientLogMssg("Executed " + cmd + " command.");
    return (true);
}

Command*    Client::searchCommand(std::string cmd) {
    commandmap::iterator cit = _commands.find(cmd);
    if (cit == _commands.end()) {
        clientLogMssg("Command '" + cmd + "' not found for the client : " + _username);
        return (NULL);
    }
    return (cit->second);
}

std::string Client::getPrefix() const {
    if (isPending())
    {
        return std::string("");
    }
    std::string prefix = _nickname;
	if (_hostname.length())
	{
		if (_username.length())
			prefix += "!" + _username;
		prefix += "@" + _hostname;
	}
	return prefix;
}

void Client::receive_reply(int code, std::string arg1, std::string arg2, std::string arg3, std::string arg4)
{
    std::stringstream str_stream;
    str_stream << code;
    std::string code_str;
    str_stream >> code_str;
    while (code_str.length() <  3)
    {
        code_str = "0" + code_str;
    }
    std::string nick;
    if (isPending())
        nick = "*";
    else
        nick = this->getNname();
    std::string reply = get_reply(code , arg1, arg2, arg3, arg4);
    std::string tosend = code_str + " " + nick + " " + reply;
    _serv->sendToClient(*this , ":" + getPrefix(), tosend);
}

void Client::getLoggedOn() {
    std::string buffer;
    buffer += ": NICK :" + this->getNname() + "\r\n";
	buffer += ":" + getPrefix() + " 001 " + this->getNname() + " " + get_reply(001, this->getNname(), this->getUname(), this->getHname(), "") + "\r\n";
	buffer += ":" + getPrefix() + " 002 " + this->getNname() + " "  + get_reply(002, this->getHname(), "0.9", "", "") + "\r\n";
	buffer += ":" + getPrefix() + " 003 " + this->getNname() + " "  + get_reply(003, "03/08/2022", "", "", "") + "\r\n";
	buffer += ":" + getPrefix() + " 004 " + this->getNname() + " "  + get_reply(004, "ft_irc", " 0.9", "", "") + "\r\n";
    serverLogMssg(buffer);
    send(getFd(), buffer.c_str(), buffer.length(), 0);
}

std::vector<std::string> Client::get_all_channels()
{
    std::vector<std::string> channames;
    for (Server::channelmap::const_iterator it = _serv->getChannels().begin(); it != _serv->getChannels().end(); ++it) {
        if (it->second.searchClient(this->getNname()))
        {
            channames.push_back(it->second.getName());
        }
    }
    return channames;
}