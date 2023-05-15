#ifndef List_HPP
# define List_HPP

#include "../Command.hpp"

class List : public Command {
    public :
        List();
        List(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif