#ifndef PrivMsg_HPP
# define PrivMsg_HPP

#include "../Command.hpp"

class PrivMsg : public Command {
    public :
        PrivMsg();
        PrivMsg(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif