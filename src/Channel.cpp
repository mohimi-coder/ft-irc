/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:09:47 by hbettal           #+#    #+#             */
/*   Updated: 2025/03/16 13:25:36 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <string>
#include <vector>

Channel ::Channel() : name(""), invOnly(false), topicMode(false), limits(0){}
Channel ::Channel(std::string name) : name(name), invOnly(false), topicMode(false), limits(0){}

std::string Channel :: get_key()
{
	return (key);
}
std::string Channel :: get_name()
{
	return (name);
}
std::vector<Client> Channel :: get_admins()
{
	return (admins);
}

std::string Channel :: get_topic()
{
	return (topic);
}

std::vector<Client>& Channel::getMembers()
{
	return (members);
}

void Channel :: set_key(std ::string key)
{
	this->key = key;
}

void Channel :: set_admin(Client user)
{
	admins.push_back(user);
}

void Channel::set_topic(std::string _topic)
{
	topic = _topic;
}

void Channel::addNewMember(Client user)
{
	this->members.push_back(user);
}

void Channel::kickMember(Client user)
{
	std::vector<Client>::iterator it = members.begin();
	while (it != members.end())
	{
		if (user.get_fd() == it->get_fd())
		{
			this->members.erase(it);
			return ;
		}
		it++;
	}
	if (isInvited(user))
	{
		std::vector<Client>::iterator it = invited.begin();
		while (it != invited.end())
		{
			if (user.get_fd() == it->get_fd())
			{
				this->invited.erase(it);
				return ;
			}
			it++;
		}
	}
}

bool Channel::memberExist(Client user)
{
	for (size_t i = 0; i < members.size(); i++)
	{
		if (user.get_fd() == members[i].get_fd())
			return true;
	}
	return false;
}

void Channel::sendWelcomeMsg(Client user, Channel room)
{
    std::vector<Client>& members = room.getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
        Server::send_msg(RPL_JOINMSG(user.get_nickName() + '!' + user.get_userName(), user.get_IpAdd(), room.get_name()), members[i].get_fd());
    }
}

void Channel::sendModeMsg(Client user, Channel room, std::string mode, std::string param)
{
    std::vector<Client> members = room.getMembers();
	if (!param.empty())
		mode = mode + ' ' + param;
    for (size_t i = 0; i < members.size(); i++)
    {
        Server::send_msg(RPL_CHANGEMODE(user.get_nickName(), room.get_name(), mode), members[i].get_fd());
    }
}

void Channel::sendKickingMsg(Client sender, Channel room, Client target, std::string comment)
{
    std::vector<Client>& members = room.getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
		if (target.get_fd() != members[i].get_fd())
			Server::send_msg(RPL_KICKMSG(sender.get_nickName() + "!~" + sender.get_userName() + "@" + sender.get_IpAdd(), room.get_name(), target.get_nickName(), comment), members[i].get_fd());
    }
	Server::send_msg(RPL_KICKDEFMSG(sender.get_nickName() + "!~" + sender.get_userName() + "@" + sender.get_IpAdd(), room.get_name(), target.get_nickName()), target.get_fd());
}

bool Channel::isAdmine(Client user)
{
	for (size_t i = 0; i < admins.size(); i++)
	{
		if (user.get_fd() == admins[i].get_fd())
			return true;
	}
	return false;
}

void Channel::removeAdmin(Client user)
{
	std::vector<Client>::iterator it = admins.begin();
	while (it != admins.end())
	{
		if (user.get_fd() == it->get_fd())
		{
			this->admins.erase(it);
			return ;
		}
		it++;
	}
}

std::string Channel::nameReply()
{
	std::string reply;
	for (size_t i = 0; i < members.size(); i++)
	{
		if (isAdmine(members[i]))
			reply += '@';
		reply += members[i].get_nickName();
		if (i + 1 < members.size())
			reply += ' ';
	}
	return reply;
}

bool Channel::isInvited(Client user)
{
	for (size_t i = 0; i < invited.size(); i++)
	{
		if (user.get_fd() == invited[i].get_fd())
			return true;
	}
	return false;
}

void Channel::inviteClient(Client user)
{
	invited.push_back(user);
}

size_t Channel::getlimits()
{
	return limits;
}

bool Channel::getInvOnlyMode()
{
	return invOnly;
}

bool Channel::getTopicMode()
{
	return topicMode;
}

void Channel::set_limits(size_t num)
{
	limits = num;
}

void Channel::set_inviteMode(bool mode)
{
	invOnly = mode;
}

void Channel::set_topicMode(bool mode)
{
	topicMode = mode;
}
