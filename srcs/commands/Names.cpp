#include "commands/Names.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Names::Names() {
}

Names::Names(Server *serv) : Command(serv) {

}

std::string Names::help_msg() const {
    return ("/Names [channel] (Names users on channel or on the server if no chaannel specified)");
}

void Names::execute(std::string line, Client &user) {
    bool blank = true;
    std::string s = "";
    for (size_t i = 0; i < line.size(); i++)
        if (!isspace(line[i]))
            blank = false;
    if (blank) { //Names user on current channel
        Channel *chan = user.getChannel();
        if (chan)
        { 
            Channel::clientlist::const_iterator it = chan->getClients().begin();
            Channel::clientlist::const_iterator ite = chan->getClients().end();
            while (it != ite) {
                user.receive_reply(353, chan->getName(), it->first);
				it++;
            }
            user.receive_reply(366, chan->getName());
        }
        else
            user.receive_reply(442, "");
    } else { //Names users on channel
        std::vector<std::string> channames = ftirc_split(line, ", ");
        for (std::vector<std::string>::const_iterator it = channames.begin(); it != channames.end(); ++it) {
            Channel *chan = _serv->searchChannel(line);
            if (chan) {
                Channel::clientlist::const_iterator it = chan->getClients().begin();
                Channel::clientlist::const_iterator ite = chan->getClients().end();
                while (it != ite) {
                    user.receive_reply(353, chan->getName(), it->first);
					it++;
                }
                user.receive_reply(366, chan->getName());
            }
        }
        
    }
}