#ifndef Oper_HPP
# define Oper_HPP

#include "../Command.hpp"


class Oper : public Command {
    public :
        Oper();
        Oper(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif
