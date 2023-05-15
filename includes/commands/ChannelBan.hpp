#ifndef ChannelBan_HPP
# define ChannelBan_HPP

#include "../Command.hpp"

class ChannelBan : public Command {
    public :
        ChannelBan();
        ChannelBan(Server *serv);

        std::string help_msg() const;
        void    execute(std::string line, Client &user);
};

#endif
