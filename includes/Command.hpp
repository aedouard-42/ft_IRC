#ifndef COMMAND_HPP
# define COMMAND_HPP
# include <string>

class Server;
class Client;

class Command {
    protected :
        Server *_serv;
    public :
        Command() {}
        Command(Server *serv) : _serv(serv) {}
        virtual ~Command() {}

        bool hasServer() {
            if (_serv)
                return (true);
            return (false);
        }

        virtual std::string help_msg() const = 0;
        virtual void    execute(std::string line, Client &user) = 0;
};

#endif
