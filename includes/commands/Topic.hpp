#ifndef Topic_HPP
# define Topic_HPP
#include "../Command.hpp"

class Topic : public Command {
    public :
        Topic();
        Topic(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif