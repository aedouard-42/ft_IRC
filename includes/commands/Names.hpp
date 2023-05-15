#ifndef Names_HPP
# define Names_HPP

#include "../Command.hpp"

class Names : public Command {
    public :
        Names();
        Names(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif