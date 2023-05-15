#ifndef Quit_HPP
# define Quit_HPP

#include "../Command.hpp"


class Quit : public Command {
    public :
        Quit();
        Quit(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif