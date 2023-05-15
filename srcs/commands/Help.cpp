/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:47 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:49 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Help.hpp"
#include "Server.hpp"

Help::Help() {
}

Help::Help(Server *serv) : Command(serv) {

}

std::string Help::help_msg() const {
    return ("/help [command] (show usage of command, or available commands if none specified)");
}

void Help::execute(std::string line, Client &user) {
    bool blank = true;
    std::string s = "Available commands :\n";
    for (size_t i = 0; i < line.size(); i++)
        if (!isspace(line[i]))
            blank = false;
    if (blank) {
        Client::commandmap::const_iterator it = user.getCommands().begin();
        Client::commandmap::const_iterator ite = user.getCommands().end();
        while (it != ite) {
            s += it->first + "\n";
            ++it;
        }
        _serv->sendToClient(user, s);
    } else {
        Command *cmd = user.searchCommand(line);
        if (cmd)
            _serv->sendToClient(user, cmd->help_msg());
    }
}