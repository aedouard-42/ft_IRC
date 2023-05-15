#ifndef Usercmd_HPP
# define Usercmd_HPP

#include "../Command.hpp"

class Usercmd : public Command {
    public :
        Usercmd();
        Usercmd(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif