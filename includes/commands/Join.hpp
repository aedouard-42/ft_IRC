#ifndef Join_HPP
# define Join_HPP

#include "../Command.hpp"

class Join : public Command {
    public :
        Join();
        Join(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif