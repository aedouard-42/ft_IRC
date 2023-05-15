#include "commands/Usercmd.hpp"
#include "Client.hpp"

Usercmd::Usercmd() {
}

Usercmd::Usercmd(Server *serv) : Command(serv) {

}

std::string Usercmd::help_msg() const {
    return ("/Usercmd newname (allows you to change your username in the server)");
}

void Usercmd::execute(std::string line, Client &user) {
    size_t pos = line.find(':');
    std::vector<std::string> splits = ftirc_split(line, " \t");
    if (pos != std::string::npos && splits.size() >= 4)
    {
        user.update_all_name(splits[0], splits[1], splits[2], line.substr(pos + 1));
		user.validateUser();
        serverLogMssg("Right number of arguments. USER executed");
    } else if (splits.size() == 4)
    {
		user.update_all_name(splits[0], splits[1], splits[2], splits[3]);
		user.validateUser();
        serverLogMssg("Right number of arguments. USER executed");
    }
    else {
        serverLogMssg("Wrong number of arguments for USER");
    }
}