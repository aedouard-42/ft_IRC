#ifndef Help_HPP
# define Help_HPP

#include "../Command.hpp"

class Help : public Command {
    public :
        Help();
        Help(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif