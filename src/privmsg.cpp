/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:08:09 by zait-bel          #+#    #+#             */
/*   Updated: 2025/03/17 10:02:08 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"


void send_channel(std::string receiver, std::string msg, Client sender, Channel *room)
{
	if (!room)
	{
		Server::send_msg(ERR_NOSUCHCHANNEL(receiver), sender.get_fd());
		return;
	}
	if (!room->memberExist(sender))
	{
		Server::send_msg(ERR_CANNOTSENDTOCHAN(sender.get_nickName(), room->get_name()), sender.get_fd());
		return;
	}
	std::vector<Client> members = room->getMembers();
	for(size_t i = 0; i < members.size(); i++)
	{
		if (sender.get_fd() != members[i].get_fd())
		{
			Server::send_msg(RPL_PRIVMSG(sender.get_nickName() + "!" + sender.get_userName() + "@" + sender.get_IpAdd(), receiver, msg), members[i].get_fd());
		}
	}
}

void send_user(std::string receiver, std::string msg, Client sender, Client *receiv)
{
	if (!receiv)
	{
		Server::send_msg(ERR_NOSUCHNICK(receiver), sender.get_fd());
		return;
	}
	Server::send_msg(RPL_PRIVMSG(sender.get_nickName() + "!" + sender.get_userName() + "@" + sender.get_IpAdd(), receiv->get_nickName(), msg), receiv->get_fd());
}

void Server::privmsg(std::string data, Client user)
{
	size_t found = data.find(':');
	std::vector<std::string> command = Server::split(data, ' ');
	
	if (command.size() < 3)
	{  
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), user.get_fd());
		return ;
	}
	
	std::string msg ;
	if (found == std::string::npos)
		msg = command[2];
	else
		msg = data.substr(found + 1, data.size());

	if (msg.empty())
	{
		Server::send_msg((ERR_NOTEXTTOSEND()), user.get_fd());
		return ;
	}

	std::vector<std::string> receiver = Server::split(command[1], ',');
	for (size_t i = 0; i < receiver.size(); i++)
	{
		if (receiver[i][0] == '#')
			send_channel(receiver[i], msg, user, getChannel(receiver[i]));
		else
			send_user(receiver[i], msg, user, getClient(receiver[i]));
	}
}
