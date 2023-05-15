#include "commands/Kick.hpp"
#include "Server.hpp"

Kick::Kick() {
}

Kick::Kick(Server *serv) : Command(serv) {
}

std::string Kick::help_msg() const {
    return ("/kick [channel] <nickname>:<message> (allows you to kick this user from the channel)");
}

void Kick::execute(std::string line, Client &user) {
    std::vector<std::string> params = ftirc_split(line, " ");
    Channel *chan = user.getChannel();
    if (params.size() < 2)
    {
        user.receive_reply(461, "KICK");
        return;
    }
    std::string channame = params[0];
    std::string usertokick = params[1];
    if (!chan)
    {
        user.receive_reply(442, channame);
        return;
    } 
    if (_serv->searchChannel(channame) == NULL)
        user.receive_reply(403, channame);

    Client *tokick = chan->searchClient(usertokick);
    if (!tokick) {
        user.receive_reply(441, usertokick, channame);
        return ;
    }
    chan->kickFromChannel(tokick, user);
}