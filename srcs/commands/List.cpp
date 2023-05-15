#include "commands/List.hpp"
#include "Server.hpp"
#include "Channel.hpp"

List::List() {
}

List::List(Server *serv) : Command(serv) {

}

std::string List::help_msg() const {
    return ("/list [channel] (list all channels on the server if no channel specified)");
}

void List::execute(std::string line, Client &user) {
    bool blank = true;
    std::string s = "";
    for (size_t i = 0; i < line.size(); i++)
        if (!isspace(line[i]))
            blank = false;
    if (blank) { //list channels on server
        Server::channelmap::const_iterator it = _serv->getChannels().begin();
        Server::channelmap::const_iterator ite = _serv->getChannels().end();
        while (it != ite) {
            std::string str;
            std::stringstream ss;  
            ss << it->second.getClients().size();  
            ss >> str;
            user.receive_reply(322, it->second.getName(), str, it->second.getTopic());
            ++it;
        }
        user.receive_reply(323);
    } else { //list the channel and his topic
        std::vector<std::string> channames = ftirc_split(line, ", ");
        for (std::vector<std::string>::const_iterator it = channames.begin(); it != channames.end(); ++it) {
            Channel *chan = _serv->searchChannel(*it);
            if (chan) {
                std::string str;
                std::stringstream ss;  
                ss << chan->getClients().size();  
                ss >> str;
                user.receive_reply(322, chan->getName(), str, chan->getTopic());
            }
        }
        user.receive_reply(323);
    }
}