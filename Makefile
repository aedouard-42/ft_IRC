# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/22 17:56:55 by lnelson           #+#    #+#              #
#    Updated: 2022/08/04 01:43:36 by lnelson          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#							FT_IRC MAKEFILE									   #
# **************************************************************************** #

NAME = ircserv

SRCS = srcs/main.cpp

CXX = c++

OBJS = ${SRCS:.cpp=.o}

INCL_DIR = includes/

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -I ${INCL_DIR}

all : $(NAME)

std: ${NAME_STD}

INCL		= ${INCL_DIR}ft_irc.hpp

SRCS		=	srcs/main.cpp \
				srcs/utils.cpp \
				srcs/Client.cpp \
				srcs/Channel.cpp \
				srcs/commands/Help.cpp \
				srcs/commands/Notice.cpp \
				srcs/commands/Join.cpp \
				srcs/commands/ChannelBan.cpp \
				srcs/commands/Nick.cpp \
				srcs/commands/Topic.cpp \
				srcs/Server.cpp \
				srcs/commands/List.cpp \
				srcs/commands/Usercmd.cpp \
				srcs/commands/Quit.cpp \
				srcs/commands/Ping.cpp \
				srcs/commands/Kick.cpp \
				srcs/commands/Names.cpp \
				srcs/commands/Pass.cpp \
				srcs/commands/PrivMsg.cpp \
				srcs/commands/Oper.cpp \
				srcs/codes.cpp\
				srcs/commands/Part.cpp\


OBJS		= ${SRCS:.cpp=.o}

.cpp.o :
		${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
		${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

sanitize: ${OBJS}
		${CXX} ${CXXFLAGS} -fsanitize=address -o sserver ${OBJS}

clean :
		rm -rf ${OBJS}

fclean: clean
		rm -rf ${NAME}

re:		fclean all

.PHONY: all clean fclean res
