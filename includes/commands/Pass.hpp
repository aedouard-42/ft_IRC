#ifndef Pass_HPP
# define Pass_HPP

#include "../Command.hpp"

class Pass : public Command {
    public :
        Pass();
        Pass(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);

        class WrongPassException : public std::exception {
            public :
                virtual const char * what(void) const throw();
        };
};

#endif