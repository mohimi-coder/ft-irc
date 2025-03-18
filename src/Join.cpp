/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:25:17 by zait-bel          #+#    #+#             */
/*   Updated: 2025/03/16 14:02:03 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include <cstddef>
#include <vector>

void check_key(std::string pass, Client *user, Channel *room, int fd)
{
	if (room->memberExist(*user))
		return;
	if (pass == room->get_key())
    {
		Server::send_msg(RPL_JOIN(user->get_nickName(), room->get_name()), user->get_fd());
		room->addNewMember(*user);
        Channel::sendWelcomeMsg(*user, *room);
		if (room->get_topic().empty())
			Server::send_msg(RPL_NOTOPIC(user->get_nickName(), room->get_name()), user->get_fd());
		else
			Server::send_msg(RPL_TOPIC(user->get_nickName(), room->get_name(), room->get_topic()), user->get_fd());
		Server::send_msg(RPL_NAMREPLY(user->get_nickName(), room->get_name(), room->nameReply()), user->get_fd());
		Server::send_msg(RPL_ENDOFNAMES(user->get_nickName(), room->get_name()), user->get_fd());
    }
	else
		Server::send_msg(ERR_BADCHANNELKEY(user->get_nickName(), room->get_name()), fd);
}

void Server::part(std::string data, Client client)
{
	std::vector<std::string> cmd = Server::split(data, ' ');
	if (cmd.size() == 3 && cmd[2] == "Leaving...")
	{
		Channel *room = getChannel(cmd[1]);
		if (room)
		{
			std::vector<Client>& members = room->getMembers();
			std::vector<Client>::iterator it = members.begin();
			while (it != members.end())
			{
				if (client.get_fd() == it->get_fd())
				{
					it = members.erase(it);
					break;
				}
				Server::send_msg(RPL_PART(client.get_nickName(),room->get_name(), ""), it->get_fd());
				it++;
			}
			Server::send_msg(RPL_PART(client.get_nickName(), room->get_name(), cmd[2]), client.get_fd());
		}
	}
}
void Server::leaveChannels(Client *user)
{
    for (size_t i = 0; i < this->__channels.size(); i++)
    {
        std::vector<Client>& members = __channels[i].getMembers();
        std::vector<Client>::iterator it = members.begin();
        while (it != members.end())
        {
            if (user->get_fd() == it->get_fd())
            {
                it = members.erase(it);
                break;
            }
			Server::send_msg(RPL_PART(user->get_nickName(), __channels[i].get_name(), ""), it->get_fd());
            it++;
        }
		Server::send_msg(RPL_PART(user->get_nickName(), __channels[i].get_name(), ""), user->get_fd());
    }
}

void Server::join(int fd, std::string data, Client *user)
{  
	std::vector<std::string> command = Server::split(data, ' ');

	if (command.size() < 2 || command[1].size() < 2)
	{
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), user->get_fd());
		return ;
	}
	std::vector<std::string> name = Server::split(command[1], ',');
	std::vector<std::string> pass(name.size());
	if (command.size() > 2)
		pass = Server::split(command[2], ',');
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name[i][0] != '#')
		{
			Server::send_msg(ERR_BADCHANMASK(name[i]), user->get_fd());
			return ;
		}
		if (name[i] == "#0")
		{
			this->leaveChannels(user);
			continue ;
		}
		Channel *room = getChannel(name[i]);
		if (!room)
		{
			Channel room(name[i]);
			room.set_admin(*user);
			room.addNewMember(*user);
			this->__channels.push_back(room);
			Server::send_msg(RPL_JOIN(user->get_nickName(), room.get_name()), user->get_fd());
			Server::send_msg(RPL_NAMREPLY(user->get_nickName(), room.get_name(), room.nameReply()), user->get_fd());
			Server::send_msg(RPL_ENDOFNAMES(user->get_nickName(), name[i]), user->get_fd());
		}
		else
		{
			if (room->getInvOnlyMode() && !room->isInvited(*user))
			{
				Server::send_msg(ERR_INVITEONLYCHAN(user->get_nickName(), room->get_name()), user->get_fd());
				continue ;
			}
			if (room->getlimits() && room->getMembers().size() + 1 > room->getlimits())
			{
				Server::send_msg(ERR_CHANNELISFULL(user->get_nickName(), room->get_name()), user->get_fd());
				continue ;
			}
			check_key(pass[i], user, room, fd);
		}
	}
}
