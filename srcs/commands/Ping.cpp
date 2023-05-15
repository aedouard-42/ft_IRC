#include "commands/Ping.hpp"
#include "Server.hpp"

Ping::Ping() {
}

Ping::Ping(Server *serv) : Command(serv) {
}

std::string Ping::help_msg() const {
    return ("/ping <token> (allows you to ping this server, it should respond with the same token)");
}

void Ping::execute(std::string line, Client &user) {
    _serv->sendToClient(user, ":" + user.getPrefix(), "PONG :" + line);
}