/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 18:02:55 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/13 18:02:57 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Oper.hpp"
#include "Client.hpp"

Oper::Oper() {
}

Oper::Oper(Server *serv) : Command(serv) {

}

std::string Oper::help_msg() const {
    return ("/oper <password> (allows you to become an operator in the server)");
}

void Oper::execute(std::string line, Client &user) {
    std::vector<std::string> params = ftirc_split(line, " ");
    if (params.size() < 2)
    {
        user.receive_reply(461, "OPER");
        return;
    }
    if (_serv->checkOpPass(params[1])) {
        user.becomeOperator();
        user.receive_reply(381);
        //_serv->sendToClient(user, "You have become an operator.");
        serverLogMssg("There is a new operator on the server.");
    }
    else
        _serv->sendToClient(user, "Wrong password.");
}