#include "commands/Quit.hpp"
#include "Client.hpp"

Quit::Quit() {
}

Quit::Quit(Server *serv) : Command(serv) {

}

std::string Quit::help_msg() const {
    return ("/Quit (allows you to Quit the server)");
}

void Quit::execute(std::string line, Client &user) {
    clientLogMssg(line);
    std::vector<std::string> channames;
    for (Server::channelmap::const_iterator it = _serv->getChannels().begin(); it != _serv->getChannels().end(); ++it) {
        if (it->second.searchClient(user.getNname()))
        {
            channames.push_back(it->second.getName());
        }
    }
    for (std::vector<std::string>::const_iterator it = channames.begin(); it != channames.end(); ++it) {
            Channel *chan = _serv->searchChannel(*it);
            if (chan)
                chan->kickFromChannel(&user, user);
        }
    _serv->deleteClient(user.getNname());
}