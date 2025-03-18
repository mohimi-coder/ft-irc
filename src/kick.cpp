/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:28:50 by zait-bel          #+#    #+#             */
/*   Updated: 2025/03/16 09:52:53 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include <cstddef>
#include <cstdio>
#include <vector>

void Server::kick(std::string data, Client user)
{
	std::vector<std::string> command = Server::split(data, ' ');
    if (command.size() < 3)
	{  
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), user.get_fd());
		return ;
	}
    size_t  found = data.find(':');
    if (command[1][0] != '#')
    {
        Server::send_msg(ERR_BADCHANMASK(command[1]), user.get_fd());
		return ;
    }
    Channel *room = getChannel(command[1]);
    if (!room)
    {
        Server::send_msg(ERR_NOSUCHCHANNEL(command[1]), user.get_fd());
		return;
    }
    if (!room->memberExist(user))
    {
        Server::send_msg(ERR_NOTONCHANNEL(user.get_nickName(), command[1]), user.get_fd());
		return ;
    }
    if (!room->isAdmine(user))
    {
        Server::send_msg(ERR_CHANOPRIVSNEEDED(command[1]), user.get_fd());
		return ;
    }
    std::vector<std::string> names = Server::split(command[2], ',');
    for (size_t i = 0; i < names.size(); i++)
    {
        Client *ban = getClient(names[i]);
        if (!ban || !room->memberExist(*ban))
            Server::send_msg(ERR_USERNOTINCHANNEL(user.get_nickName(), command[1]), user.get_fd());
        else
        {
            room->kickMember(*ban);
            if (found == std::string::npos && command.size() > 3)
                Channel::sendKickingMsg(user, *room, *ban, command[3]);
            else if (found != std::string::npos && command[3].size() > 1)
                Channel::sendKickingMsg(user, *room, *ban, data.substr(found + 1));
            else
                Channel::sendKickingMsg(user, *room, *ban, "");
        }
    }
}
