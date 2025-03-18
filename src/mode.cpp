/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohimi <mohimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:32:42 by zait-bel          #+#    #+#             */
/*   Updated: 2025/03/17 23:32:59 by mohimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include <cctype>
#include <cstddef>

bool isDigit(std::string str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

void Server::mode(std::string data, Client user)
{
	std::vector<std::string> command = Server::split(data, ' ');
	
	if (command[1][0] == '#')
	{	
		Channel *room = getChannel(command[1]);
		if (!room)
		{
			Server::send_msg(ERR_NOSUCHCHANNEL(command[1]), user.get_fd());
			return;
		}
		
		if (command.size() < 3 || command[2].size() < 2 || (command[2][0] != '+' && command[2][0] != '-'))
		{
			Server::send_msg(RPL_CHANNELMODEIS(user.get_nickName(), room->get_name(), "+/-", "i/t/k/o/l"), user.get_fd());
			return;
		}
		if (!room->memberExist(user))
		{
			Server::send_msg(ERR_NOTONCHANNEL(user.get_nickName(), room->get_name()), user.get_fd());
			return ;
		}
		if (!room->isAdmine(user))
		{
			Server::send_msg(ERR_CHANOPRIVSNEEDED(room->get_name()), user.get_fd());
			return ;
		}
		bool isPos = command[2][0] == '+';
		size_t curr = 3;
		for (size_t i = 1; i < command[2].size(); i++)
		{
			if (command[2][i] == '+')
				isPos = true;
			else if (command[2][i] == '-')
				isPos = false;
			else if (isPos)
			{
				if (command[2][i] == 'i')
				{
					room->set_inviteMode(true);
					Channel::sendModeMsg(user, *room, "+i", "");
				}
				else if (command[2][i] == 't')
				{
					room->set_topicMode(true);
					Channel::sendModeMsg(user, *room, "+t", "");
				}
				else if (command[2][i] == 'k' && command.size() > curr)
				{
					room->set_key(command[curr]);
					Channel::sendModeMsg(user, *room, "+k", command[curr]);
					curr++;
				}
				else if (command[2][i] == 'o' && command.size() > curr)
				{
					Client *member = getClient(command[curr]);
					if (!member || !room->memberExist(*member))
					{
						Server::send_msg(ERR_USERNOTINCHANNEL(user.get_nickName(), room->get_name()), user.get_fd());
						curr++;
						continue ;
					}
					room->set_admin(*member);
					Channel::sendModeMsg(user, *room, "+o", command[curr]);
					curr++;
				}
				else if (command[2][i] == 'l' && command.size() > curr && isDigit(command[curr]))
				{
					room->set_limits(std::atol(command[curr].c_str()));
					Channel::sendModeMsg(user, *room, "+l", command[curr]);
					curr++;
				}
			}
			else if (!isPos)
			{
				if (command[2][i] == 'i')
				{
					room->set_inviteMode(false);
					Channel::sendModeMsg(user, *room, "-i", "");
				}
				else if (command[2][i] == 't')
				{
					room->set_topicMode(false);
					Channel::sendModeMsg(user, *room, "-t", "");	
				}
				else if (command[2][i] == 'k')
				{
					room->set_key("");
					Channel::sendModeMsg(user, *room, "-k", "");	
				}
				else if (command[2][i] == 'o' && command.size() > curr)
				{
					Client *member = getClient(command[curr]);
					if (!member || !room->memberExist(*member))
					{
						Server::send_msg(ERR_USERNOTINCHANNEL(user.get_nickName(), room->get_name()), user.get_fd());
						curr++;
						continue ;
					}
					room->removeAdmin(*member);
					Channel::sendModeMsg(user, *room, "-o", command[curr]);
					curr++;
				}
				else if (command[2][i] == 'l')
				{
					room->set_limits(0);
					Channel::sendModeMsg(user, *room, "-l", "");	
				}
			}
			else
				Server::send_msg(ERR_UMODEUNKNOWNFLAG(user.get_nickName()), user.get_fd());
		}
	}
}
