/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:34:31 by zait-bel          #+#    #+#             */
/*   Updated: 2025/03/17 17:29:57 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"



void Server::topic(std::string data, Client *user)
{
	std::vector<std::string> topic = Server::split(data, ' ');
	if (topic.size() < 2)
	{
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), user->get_fd());
		return ;
	}
	if (topic[1][0] != '#')
	{
		Server::send_msg(ERR_BADCHANMASK(topic[1]), user->get_fd());
		return ;
	}
	Channel *room = getChannel(topic[1]);
	if (!room)
	{
		Server::send_msg(ERR_NOSUCHCHANNEL(topic[1]), user->get_fd());
		return;
	}

	if (!room->memberExist(*user))
	{
		Server::send_msg(ERR_NOTONCHANNEL(user->get_nickName(), topic[1]), user->get_fd());
		return ;
	}
	if (topic.size() < 3)
	{
		if (room->get_topic().empty())
			Server::send_msg(RPL_NOTOPIC(user->get_nickName(), topic[1]), user->get_fd());
		else
			Server::send_msg(RPL_TOPIC(user->get_nickName(), topic[1], room->get_topic()), user->get_fd());
	}
	else if (topic[2][0] == ':')
	{
		if (topic[2].size() == 1 && topic.size() == 3)
			room->set_topic("");
		else if (!room->getTopicMode() || (room->getTopicMode() && room->isAdmine(*user)))
			room->set_topic(data.substr(data.find(':') + 1));
		else
			Server::send_msg(ERR_CHANOPRIVSNEEDED(user->get_nickName()), user->get_fd());
	}
	else
	{
		if (!room->getTopicMode() || (room->getTopicMode() && room->isAdmine(*user)))
			room->set_topic(topic[2]);
		else
			Server::send_msg(ERR_CHANOPRIVSNEEDED(user->get_nickName()), user->get_fd());
	}
}
