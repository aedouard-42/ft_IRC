#ifndef Ping_HPP
# define Ping_HPP

#include "../Command.hpp"

class Ping : public Command {
    public :
        Ping();
        Ping(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif