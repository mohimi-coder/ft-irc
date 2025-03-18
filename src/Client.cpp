/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:59:33 by mohimi            #+#    #+#             */
/*   Updated: 2025/03/13 13:10:25 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
    __fDs = 0;
    __ip_add = "";
    __user_name = "";
    __nick_name = "";
    __hasPass = false;
    __isRegistred = false;
    __hasUser = false;
    __hasNick = false;  
}

Client::~Client()
{
}

Client::Client(int fd, std::string ipadd, std::string nick, std::string user)
{
    __fDs = fd;
    __ip_add = ipadd;
    __user_name = user;
    __nick_name = nick;
}

Client &Client::operator=(const Client &src)
{
    if (this != &src)
    {
        __fDs = src.__fDs;
        __ip_add = src.__ip_add;
        __user_name = src.__user_name;
        __nick_name = src.__nick_name;
    }
    return *this;
}

Client::Client(const Client &src)
{
    *this = src;
}

int            Client::get_fd() const
{
    return __fDs;
}

std::string    Client::get_IpAdd() const
{
    return __ip_add;
}

std::string &Client::get_nickName()
{
    return __nick_name;
}

std::string &Client::get_userName()
{
    return __user_name;
}

bool           Client::get_hasPass() const
{
    return __hasPass;
}

void        Client::set_Fd(int fd)
{
    __fDs = fd;
}

void        Client::set_IpAdd(std::string ipadd)
{
    __ip_add = ipadd;
}

void    Client::set_userName(std::string user_name)
{
    __user_name = user_name;
}

void Client::set_nickName(std::string nick_name)
{
    __nick_name = nick_name;
}

void Client::set_hasPass(bool hasPass)
{
    __hasPass = hasPass;
}

void Client::set_isRegistred(bool is_registred)
{
    __isRegistred = is_registred;
}

bool Client::get_isRegistred() const
{
    return __isRegistred;
}

void Client::set_hasUser(bool has_user)
{
    __hasUser = has_user;
}

bool Client::get_hasUser() const
{
    return __hasUser;
}

void Client::set_hasNick(bool has_nick)
{
    __hasNick = has_nick;
}

bool Client::get_hasNick() const
{
    return __hasNick;
}

std::string &Client::get_buffer()
{
    return __buffer;
}
