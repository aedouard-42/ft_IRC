#include "commands/PrivMsg.hpp"
#include "Server.hpp"

Notice::Notice() {
}

Notice::Notice(Server *serv) : Command(serv) {

}

std::string Notice::help_msg() const {
    return ("/Notice <target> :<message> (allows you to send a private msg to someone.)");
}

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

void Notice::execute(std::string line, Client &user) {
    size_t pos = line.find(":");
    std::string target;
    //pos == npos todo
    if (*(line.begin()) == '#')
    {
        target = line.substr(0, pos - 1);
        target = trim(target);
        serverLogMssg(target);
        Channel *chan = _serv->searchChannel(target);
        if (chan && chan->searchClient(user.getNname()) != NULL)
        {
            for (Channel::clientlist::const_iterator it = chan->getClients().begin(); it != chan->getClients().end(); ++it)
            {
                if ((it->second)->getNname() != user.getNname())
                    _serv->sendToClient(*(it->second), ":" + user.getPrefix(), "NOTICE " + line);
            }
        }
        else if (!chan)
            user.receive_reply(403, target);
        else
			user.receive_reply(442, chan->getName());
    }
    else
    {
        target = line.substr(0, pos - 1);
		target = trim(target);
        Client *tar = _serv->searchClient(target);
        if (!tar) {
            serverLogMssg("User not found error to transmit, user nickname =|" + target + "|");
            return ;
        }
        _serv->sendToClient(*tar, ":" + user.getPrefix(), "NOTICE " + target + " :" + line.substr(pos + 1));
    }
}