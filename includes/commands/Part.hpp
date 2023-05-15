#ifndef PART_HPP
# define PART_HPP

# include <iostream>
# include <string>
#include "../Command.hpp"

class Part : public Command
{
	public:
		Part();
		Part(Server *serv);
		~Part();

	std::string help_msg() const;
	void    execute(std::string line, Client &user);
};


#endif /* ************************************************************* PART_HPP */
