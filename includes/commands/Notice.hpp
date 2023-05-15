#ifndef Notice_HPP
# define Notice_HPP

#include "../Command.hpp"

class Notice : public Command {
    public :
        Notice();
        Notice(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif