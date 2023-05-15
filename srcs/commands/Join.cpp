/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:50 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:51 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Join.hpp"
#include "Server.hpp"
#include <string>
#include <iostream>
#include <sstream>

Join::Join() {
}

Join::Join(Server *serv) : Command(serv) {

}

std::string Join::help_msg() const {
    return ("/join <channel> (allows you to join this channel, and creates it if it does not exist yet)");
}

void Join::execute(std::string line, Client &user) {
    std::string channame = line;
    std::vector<std::string> params = ftirc_split(line, " ");
    if (params.size() == 0)
    {
        user.receive_reply(461, "JOIN");
        return;
    }
    Channel toadd = Channel(_serv, channame);
    Server::channelmap::iterator it = _serv->addChannel(toadd);
    if (it->second.addClient(&user)) {
        user.changeChannel(&(it->second));
        _serv->sendToClient(user, "You joined the channel.");
        serverLogMssg("A user has changed channel.");
    } else {
        user.receive_reply(474, toadd.getName());
    }
}