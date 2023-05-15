/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 17:37:07 by lnelson           #+#    #+#             */
/*   Updated: 2022/08/03 17:43:44 by lnelson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	serverLogMssg(std::string to_print)
{
	*logStream << "(SERVER): " << to_print << std::endl;
}

void	clientLogMssg(std::string to_print)
{
	*logStream << "(CLIENT): " << to_print << std::endl;
}

std::vector<std::string> ftirc_split(std::string s, std::string delimiter) {
    size_t min = std::string::npos, pos_end;
    size_t tmp, pos_start = 0;
	std::string token;
    std::vector<std::string> res;
	for(std::string::iterator it = delimiter.begin(); it != delimiter.end(); ++it) {
		tmp = s.find(*it, pos_start);
		if (tmp < min) {
			min = tmp;
		}
	}
	pos_end = min;
    while (pos_end != std::string::npos && s.size() > pos_start) {
		//std::cout << pos_start << ":" << pos_end << std::endl;
		if (pos_end - pos_start) {
        	token = s.substr(pos_start, pos_end - pos_start);
        	res.push_back(token);
		}
        pos_start = pos_end + 1;
		while (s.size() > pos_start && delimiter.find(s[pos_start]) != std::string::npos) //tant que delimiter se suit
			++pos_start;
		min = std::string::npos;
		for(std::string::iterator it = delimiter.begin(); it != delimiter.end(); ++it) {
			tmp = s.find(*it, pos_start);
			//std::cout << tmp << std::endl;
			if (tmp < min) {
				min = tmp;
			}
		}
		pos_end = min;
    }
	if (s.size() > pos_start) {
        token = s.substr(pos_start, s.size() - pos_start);
        res.push_back(token);
	}
    return res;
}

/*

void	inserNewClient(std::map<int, Client> usesMap, char buff[552])
{
	std::string uname;
	std::string hostName;
	std::string realName;

	
}
*/
