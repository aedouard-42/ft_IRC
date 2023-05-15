/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelBan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:45 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:46 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/ChannelBan.hpp"
#include "Server.hpp"

ChannelBan::ChannelBan() {
}

ChannelBan::ChannelBan(Server *serv) : Command(serv) {
}

std::string ChannelBan::help_msg() const {
    return ("/ban username (allows you to ban this user from the channel)");
}

void ChannelBan::execute(std::string line, Client &user) {
    std::vector<std::string> params = ftirc_split(line, " ");
    Channel *chan = user.getChannel();
    if (params.size() < 2)
    {
        user.receive_reply(461, "BAN");
        return;
    }
    std::string channame = params[0];
    std::string usertoban = params[1];
    if (!chan)
    {
        user.receive_reply(442, channame);
        return;
    } 
    if (_serv->searchChannel(channame) == NULL)
        user.receive_reply(403, channame);

    Client *toban = chan->searchClient(usertoban);
    if (!toban) {
        user.receive_reply(441, usertoban, channame);
        return ;
    }
    
    chan->kickFromChannel(toban, user);
    chan->addToBanList(toban);
}