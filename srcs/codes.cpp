#include "commands/codes.hpp"


	/*- The server sends Replies 001 to 004 to a user upon
			successful registration.
	*/

	/*001*/
	std::string RPL_WELCOME(std::string nick, std::string user, std::string host)
	{return ":Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host;}
	/*002*/
	std::string RPL_YOURHOST(std::string servername, std::string version)
	{return ":Your host is " + servername  + ", running version " + version;}
	/*003*/
	std::string RPL_CREATED(std::string date)
	{return ":This server was created " + date;}
	/*004*/
	std::string RPL_MYINFO(std::string servername, std::string version, std::string umodes, std::string cmodes)
	{return servername + version + umodes + cmodes;}
    /*322*/
	std::string RPL_LIST(std::string channame, std::string nbr, std::string topic)
    {return channame + " " + nbr + " :" + topic;}
    /*322*/
	std::string RPL_LISTEND()
    {return ":End of LIST";}
	/*331*/
	std::string RPL_NOTOPIC(std::string channel)
	{return channel + " :No topic set";}
	/*332*/
	std::string RPL_TOPIC(std::string channel, std::string topic)
	{return channel + " :" + topic;}
	/*382*/
	std::string RPL_REHASHING(std::string config_file)
	{return config_file + " :Rehashing";}
	/*403*/
	std::string ERR_NOSUCHCHANNEL(std::string channel_name)
	{return channel_name + " :No such channel";}
	/*421*/
	std::string ERR_UNKNOWNCOMMAND(std::string command)
	{ return command + " :Unknown command"; }
	/*431*/
	std::string ERR_NONICKNAMEGIVEN()
	{return ":No nickname given";}
	/*433*/
	std::string ERR_NICKNAMEINUSE(std::string nick)
	{return nick + " :Nickname is already in use";}
	/*432*/    
	std::string ERR_ERRONEUSNICKNAME(std::string nick)
	{return nick + " :Erroneous nickname";}
	/*461*/
	std::string ERR_NEEDMOREPARAMS(std::string command)
	{return command + " :Not enough parameters";}
	/*462*/
	std::string ERR_ALREADYREGISTRED()
	{return ":Unauthorized command (already registered)";}
	/*464*/    
	std::string ERR_PASSWDMISMATCH()
	{return ":Password incorrect";}
	/*474*/    
	std::string ERR_BANNEDFROMCHAN(std::string channel)
	{return channel + " :Cannot join channel (+b)";}
    /*440*/
    //std::string ERR_NOTONACHANNEL(std::string channel)
    //{return "You're not on a channel";}
    /*442*/
    std::string ERR_NOTONCHANNEL(std::string channel)
    {return channel + " :You're not on that channel";}
    /*441*/
    std::string ERR_USERNOTINCHANNEL(std::string nickname, std::string channel)
    {return nickname + " " + channel + " :They aren't on that channel";}
    /*381*/
    std::string RPL_YOUREOPER()
    {return ":You are now an IRC operator";}
	//353
	std::string RPL_NAMREPLY(std::string channel, std::string nick)
	{return channel + " : " + nick;}
	//366
	std::string RPL_ENDOFNAMES(std::string channel)
	{return channel + " :End of NAMES list";}

    std::string get_reply(int code, std::string arg1 = std::string(""), std::string arg2 = std::string(""), std::string arg3 = std::string("") , std::string arg4 = std::string(""))
	{

	switch (code)
	{
		case 001:
			return RPL_WELCOME(arg1, arg2, arg3);
		case 002:
			return RPL_YOURHOST(arg1, arg2);
		case 003:
			return RPL_CREATED(arg1);
		case 004:
			return RPL_MYINFO(arg1,arg2,arg3,arg4);
		case 331:
			return RPL_NOTOPIC(arg1);
		case 322:
			return RPL_LIST(arg1, arg2, arg3);
		case 323:
			return RPL_LISTEND();
		case 332:
			return RPL_TOPIC(arg1, arg2);
		case 353:
			return RPL_NAMREPLY(arg1, arg2);
		case 366:
			return RPL_ENDOFNAMES(arg1);
		case 382:
			return RPL_REHASHING(arg1);
		case 403:
			return ERR_NOSUCHCHANNEL(arg1);
		case 421:
			return ERR_UNKNOWNCOMMAND(arg1);
		case 431:
			return ERR_NONICKNAMEGIVEN();
		case 433:
			return ERR_NICKNAMEINUSE(arg1);
		case 432:
			return ERR_ERRONEUSNICKNAME(arg1);
		case 461:
			return ERR_NEEDMOREPARAMS(arg1);
		case 462:
			return ERR_ALREADYREGISTRED();
		case 464:
			return ERR_PASSWDMISMATCH();
		case 474:
			return ERR_BANNEDFROMCHAN(arg1);
        case 442:
            return ERR_NOTONCHANNEL(arg1);
        case 441:
            return ERR_USERNOTINCHANNEL(arg1, arg2);
        case 381:
            return RPL_YOUREOPER();
		default:
			return "";
		}
	}

