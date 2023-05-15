/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:52 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/04 02:53:20 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Nick.hpp"
#include "Client.hpp"

Nick::Nick() {
}

Nick::Nick(Server *serv) : Command(serv) {

}

std::string Nick::help_msg() const {
    return ("/nick <nickname> (allows you to change your nickname in the server)");
}

void Nick::execute(std::string line, Client &user) {
    std::vector<std::string> params = ftirc_split(line, " ");
    if (params.size() == 0)
    {
        user.receive_reply(461, "NICK");
        return;
    }
    if (_serv->searchClient(line))
    {
        user.receive_reply(433, line);
        return ;
    }
    if (!user.isPending())
    {
        std::vector<std::string> channames;
        std::string nicksender = user.getNname();
        Channel::clientlist receivers;
		receivers.insert(std::make_pair(user.getNname(), &user));
        for (Server::channelmap::const_iterator it = _serv->getChannels().begin(); it != _serv->getChannels().end(); ++it) {
            clientLogMssg(it->second.getName());
            if (it->second.searchClient(nicksender))
            {
                channames.push_back(it->second.getName());
                clientLogMssg(it->second.getName());
                for (Channel::clientlist::const_iterator itc = it->second.getClients().begin(); itc != it->second.getClients().end(); ++itc)
                {
                    receivers.insert(*itc);
                }
            }
        }
        for (Channel::clientlist::const_iterator it = receivers.begin(); it != receivers.end(); ++it)
            _serv->sendToClient(*it->second, ":" + user.getPrefix(), "NICK :" + line);
        for (std::vector<std::string>::const_iterator it = channames.begin(); it != channames.end(); ++it) {
            Channel *chan = _serv->searchChannel(*it);
            if (chan)
                chan->changenickClient(line, nicksender);
        }

        
    }
//    else{
//        _serv->sendToClient(user, ":" + user.getPrefix(), "NICK :" + line);
//    }
    user.validateNick();
    user.changeName(line);
    serverLogMssg("Nick command executed");
}

const char * Nick::NameTakenException::what() const throw() {
    return "nickname in use";
}