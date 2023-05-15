#include "commands/Part.hpp"
#include "Server.hpp"

Part::Part()
{}

Part::Part(Server *serv)
:Command(serv)
{}

Part::~Part()
{}

std::string Part::help_msg() const {
    return ("Parameters: <canal>{,< canal >} ");
}

void Part::execute(std::string line, Client &user)
{
	std::vector<std::string> params = ftirc_split(line, ",");
    if (params.size() < 1)
    {
        user.receive_reply(461, "PART");
        return;
    }
    for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
            Channel *chan = _serv->searchChannel(*it);
            if (chan)
			{
				if (chan->searchClient(user.getNname()))
				{
					chan->Leavechannel(&user);
				}
				else
				{
					user.receive_reply(442, chan->getName());
				}
			}
			else
				user.receive_reply(403, *it);
        }
}