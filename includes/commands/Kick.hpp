#ifndef Kick_HPP
# define Kick_HPP

#include "../Command.hpp"

class Kick : public Command {
    public :
        Kick();
        Kick(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif