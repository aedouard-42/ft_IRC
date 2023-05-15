/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:40 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/04 01:29:15 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){
    _serv = NULL;
}

Channel::Channel(Server *current, std::string cname)
    : _name(cname), _topic("")
{
    _serv = current;
}

Channel::Channel(const Channel &other)
    : _name(other._name), _params(other._params), _clients(other._clients), _clientsban(other._clientsban), _topic("")
{
    _serv = other._serv;
}

Channel::~Channel(){

}

Channel&    Channel::operator=(const Channel& other){
    _serv = other._serv;
    _name = other._name;
    _params = other._params;
    _clients = other._clients;
    _clientsban = other._clientsban;
    return (*this);

}

const Channel::clientlist    &Channel::getClients() const {
    return (_clients);
}

bool        Channel::addClient(Client *toAdd){
    if (toAdd == searchBanned(toAdd->getNname()))
    {
        clientLogMssg("client can't be added (banned)");
        return (false);
    }
    _clients.insert(std::make_pair(toAdd->getNname(), toAdd));
    return true;
}

bool    Channel::addToBanList(Client *toBan)
{
    return _clientsban.insert(std::make_pair(toBan->getNname(), toBan)).second;
}

bool    Channel::kickFromChannel(Client *toKick, Client &kicker)
{
    clientlist::iterator it = _clients.find(toKick->getNname());
    if (it == _clients.end())
        return (0);
    else
    {
        _serv->sendToClient(*toKick, ":" + kicker.getPrefix(), "KICK " + getName() + " " + toKick->getNname());
        _clients.erase(it);
        return (true);
    }
}

bool    Channel::Leavechannel(Client *toKick)
{
    clientlist::iterator it = _clients.find(toKick->getNname());
    if (it == _clients.end())
        return (0);
    else
    {
        _serv->sendToClient(*toKick, ":" + toKick->getPrefix(), "PART " + getName() + " " + toKick->getNname());
        _clients.erase(it);
        return (true);
    }
}

void	Channel::changeTopic(std::string newTopic)
{
	_topic = newTopic;
}

Client      *Channel::searchClient(std::string nickname){
    clientlist::iterator it = _clients.find(nickname);
    if (it == _clients.end())
        return (NULL);
    return (it->second);
}

void    Channel::changenickClient(std::string nick, std::string oldnick) {
    clientlist::iterator it = _clients.find(oldnick);
    if (it == _clients.end())
        return ;
    _clients.insert(std::make_pair(nick, it->second));
    _clients.erase(it);
    return ;
}

bool      Channel::searchClient(std::string nickname) const{
    clientlist::const_iterator it = _clients.find(nickname);
    if (it == _clients.end())
        return (false);
    return (true);
}

Client      *Channel::searchBanned(std::string nickname){
    clientlist::iterator it = _clientsban.find(nickname);
    if (it == _clientsban.end())
        return (NULL);
    return (it->second);
}

std::string Channel::getName() const{
    return (_name);
}

const Channel::clientlist Channel::getBannedClients() const
{
    return _clientsban;
}