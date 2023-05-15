#include "commands/Topic.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Topic::Topic() {
}

Topic::Topic(Server *serv) : Command(serv) {

}

std::string Topic::help_msg() const {
    return ("/Topic <channel> [<topix>] (Change the current Topic of the given channel)");
}

static std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
static std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
static std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

void Topic::execute(std::string line, Client &user) {
	
	size_t pos = line.find(":");

	std::string targetChannel;
	std::string newTopicName;

	targetChannel = line.substr(0, pos - 1);
	targetChannel = trim(targetChannel);
		
	serverLogMssg("target channel =|" + targetChannel + "|");
	
	if (*(line.begin()) == '#')
	{
		Channel *chan = _serv->searchChannel(targetChannel);

		if (chan)
			serverLogMssg("topic name =|" + chan->getTopic() + "|");

		if (pos == line.size() || pos == std::string::npos)
		{
			if (chan && chan->searchClient(user.getNname()) != NULL && chan->getTopic() != "")
				user.receive_reply(332, targetChannel,  chan->getTopic());

			else if (!chan)
				user.receive_reply(403, targetChannel);

			else if (chan->searchClient(user.getNname()) == NULL)
				user.receive_reply(442, chan->getName());

			else if (chan->getTopic() == "")
				user.receive_reply(331, targetChannel);

			return ;
		}
		
		else
		{
			newTopicName = line.substr(pos + 1);
			newTopicName = trim(newTopicName);

			serverLogMssg("new topic name = |" + newTopicName + "|");

			if (chan && chan->searchClient(user.getNname()) != NULL)
			{
				chan->changeTopic(newTopicName);
	       	    for (Channel::clientlist::const_iterator it = chan->getClients().begin(); it != chan->getClients().end(); ++it)
        	    {
        	        if ((it->second)->getNname() != user.getNname())
        	            (it->second)->receive_reply(332, targetChannel, newTopicName);
	       	    }
				user.receive_reply(332, targetChannel, newTopicName);
			}
			else if (!chan)
				user.receive_reply(403, targetChannel);
			else
				user.receive_reply(442, chan->getName());
		}
	}
}