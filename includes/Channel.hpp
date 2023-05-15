#ifndef Channel_HPP
#define Channel_HPP
#include <map>
#include <string>
/*
** Pre-declared class needed to initialize Channels
*/
class Server;
class Client;
/*
** if we need to change clients map
struct s_client_info {
    public :
        Client *client;
        bool writing;
        bool oper;
        bool invite;
        s_client_info(){
            client = NULL;
            writing = false;
            oper = false;
            invite = false;
        }
        s_client_info()
};
*/
class Channel
{
	public:
        typedef std::map<std::string, Client *> clientlist;

	private:
		std::string							_name;
		Server*								_serv;
		std::map<std::string, bool>         _params;
		clientlist                  		_clients;
		clientlist                  		_clientsban;
		std::string							_topic;

	public:
		Channel();
		Channel(Server *current, std::string cname);
        Channel(const Channel &other);
		~Channel();
        Channel& operator=(const Channel& other);

        bool    addClient(Client *toAdd);
        bool    addToBanList(Client *toBan);
	void	changeTopic(std::string newTopic);
        bool    kickFromChannel(Client *toKick, Client &kicker);
	Client *searchClient(std::string nickname);
        bool    searchClient(std::string nickname) const;
        void    changenickClient(std::string nick, std::string oldnick);
        Client *searchBanned(std::string nickname);
        const clientlist getBannedClients() const;
        const clientlist    &getClients() const;
        bool   Leavechannel(Client *toKick);
        std::string getTopic() const{
		return (_topic);
	}
        std::string getName() const;
};

#include "Server.hpp"
#include "Client.hpp"

#endif
