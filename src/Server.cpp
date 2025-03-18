/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohimi <mohimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:23:33 by mohimi            #+#    #+#             */
/*   Updated: 2025/03/18 01:26:47 by mohimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

bool Server::__signal = false;

void Server::shutdown_sig(int signal)
{
    (void)signal;
    __signal = true;
    std::cout << RED "Server shutdown" pos << std::endl;
}

Server::Server(int port, std::string pass_word) : __port(port), __passWord(pass_word)
{
    __fd_socket = -1;
}

void Server::Server_Launcher()
{
    ServerSocket();
    while (!__signal)
        addNew_Client();
    if (__signal)
    {
        clearAll_Fds();
    }
}

void Server::ServerSocket()
{
    int opt = 1;

    __fd_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (__fd_socket == -1)
        throw std::runtime_error("Error: socket failed");
    if (setsockopt(__fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(__fd_socket);
        throw std::runtime_error("Error: setsockopt failed");
    }
    if (fcntl(__fd_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        close(__fd_socket);
        throw std::runtime_error("Error: fcntl failed");
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(__port);
    if (bind(__fd_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(__fd_socket);
        throw std::runtime_error("Error: bind failed");
    }
    if (listen(__fd_socket, 128) < 0)
    {
        close(__fd_socket);
        throw std::runtime_error("Error: listen failed");
    }
    struct pollfd pfd;
    memset(&pfd, 0, sizeof(pfd));
    std::cout << gold"Server is running on port " pos << __port << std::endl;
    std::cout << color "Server is waiting for connections" pos << std::endl;
    pfd.fd = __fd_socket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    __fDs.push_back(pfd);
}

void Server::addNew_Client()
{
    int new_socket;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    if (poll(__fDs.data(), __fDs.size(), 0) < 0)
    {
        clearAll_Fds();
        throw std::runtime_error("Error: poll failed");
    }
    if (__fDs[0].revents & POLLIN)
    {
        if ((new_socket = accept(__fd_socket, (struct sockaddr *)&addr, (socklen_t *)&addrlen)) < 0)
        {
            close(__fd_socket);
            throw std::runtime_error("Error: accept failed");
        }
        struct pollfd pfd;
        pfd.fd = new_socket;
        pfd.events = POLLIN;
        pfd.revents = 0;
        __fDs.push_back(pfd);

        Client c;
        c.set_Fd(new_socket);
        __clients.push_back(c);
        std::cout << violet "New client connected" pos << std::endl;
    }
    for (size_t i = 1; i < __fDs.size(); i++)
    {
        if (__fDs[i].revents & POLLIN)
            ReceiveNewData(__fDs[i].fd);
    }
}

void Server::ReceiveNewData(int fd)
{
    char buff[1024] = {0};
    ssize_t num_bytes = recv(fd, buff, sizeof(buff) - 1, 0);
    std::string data;
    Client *client = NULL;
    if (num_bytes <= 0)
    {
        if (num_bytes == 0)
        {
            std::cout << b_italic color "==>Client <" pos << fd << b_italic RED "> Disconnected" pos<< std::endl;
            for (size_t i = 0; i < __clients.size(); i++)
            {
                if (__clients[i].get_fd() == fd)
                {
                    __clients[i].set_isRegistred(false);
                    __clients[i].set_hasNick(false);
                    __clients[i].set_hasPass(false);
                    __clients[i].set_hasUser(false);
                    close(fd);
                    __clients.erase(__clients.begin() + i);
                    break;
                }
            }
        }
        else
            std::cout << b_italic color "==>Client <" pos << fd << b_italic RED "> Disconnected" pos << std::endl;
        return ;
    }
    if (num_bytes < 1024)
        buff[num_bytes] = '\0';
    for (size_t i = 0; i < __clients.size(); i++)
    {
        if (__clients[i].get_fd() == fd)
        {   
            client = &__clients[i];
            break;
        }
    }
    if (client == NULL)
        return ;
    client->get_buffer().append(buff);
    size_t poss;
    if ((poss = client->get_buffer().find('\n')) != std::string::npos)
    {
        std::string command = client->get_buffer().substr(0, poss);
        client->get_buffer().erase(0, poss + 1);
        rmoveNew_line(command);
        handleCommands(fd, command, client);
    }
    else 
        client->get_buffer().append(" ");
    if (!client->get_isRegistred() && client->get_hasPass() && client->get_hasNick() && client->get_hasUser())
    {
        client->set_isRegistred(true);
        send_msg(RPL_WELCOME(client->get_nickName(), "Welcome to the IRC server"), fd);
    }
    
}

void Server::handleCommands(int fd, std::string &data, Client *client)
{
    if (!std::strncmp(data.c_str(), "PASS ", 5))
        passWord(fd, data);
	else if (!std::strncmp(data.c_str(), "USER ", 5))
        userName(fd, data);
    else if (!std::strncmp(data.c_str(), "NICK ", 5))
        nickName(fd, data);
    else if (!std::strncmp(data.c_str(), "JOIN ", 5))
        join(fd, data, client);
    else if (!std::strncmp(data.c_str(), "TOPIC ", 5))
		topic(data, client);
    else if (!std::strncmp(data.c_str(), "INVITE ", 7))
        invite(data, *client);
    else if (!std::strncmp(data.c_str(), "PRIVMSG ", 8))
        privmsg(data, *client);
    else if (!std::strncmp(data.c_str(), "KICK ", 5))
        kick(data, *client);
    else if (!std::strncmp(data.c_str(), "MODE ", 5))
        mode(data, *client);
    else if (!std::strncmp(data.c_str(), "PART ", 5))
		part(data, *client);
    else
        Server::send_msg(ERR_UNKNOWNCOMMAND(client->get_nickName(), data), client->get_fd());
}

void Server::clearAll_Fds()
{
    close(__fd_socket);
    for (size_t i = 0; i < __fDs.size(); i++)
    {
        close(__fDs[i].fd);
        __fDs.erase(__fDs.begin() + i);
    }
}

Server::~Server()
{
    clearAll_Fds();
}

int Server::get_Fdsocket()
{
    return __fd_socket;
}

std::vector<Client>::iterator Server::get_client(int fd)
{
    for (std::vector<Client>::iterator it = __clients.begin(); it != __clients.end(); it++)
        if (it->get_fd() == fd)
            return it;
    return __clients.end();
}

void Server::userName(int fd, std::string data)
{
    std::string user = data.substr(5, data.size() - 5);
    std::string valid_user;
    std::string name;
    int count = 0;
    std::vector<std::string> sv = split(user, ' ');
    if (!sv.empty())
    {
        if(parce_nick(sv[0]))
            valid_user = sv[0];
        else {
            send_msg(ERR_ERRUSER(sv[0]), fd);
            return ;
        }
        
    }
    for(std::vector<std::string>::iterator it = sv.begin(); it != sv.end(); it++)
        count++;
    std::vector<Client>::iterator it = get_client(fd);
    if (it->get_hasPass() && count == 4)
    {
        it->set_userName(valid_user);
        it->set_hasUser(true);
    }
    else {
        send_msg(ERR_NOTREGISTERED, fd);
    }
}

std::vector<Client>::iterator Server::client_nick(std::string nick_name)
{
    for (std::vector<Client>::iterator it = __clients.begin(); it != __clients.end(); it++)
        if (it->get_nickName() == nick_name)
            return it;
    return __clients.end();
}

void Server::rmoveNew_line(std::string &str)
{
    std::string s = str;
    int i = str.size() - 1;
    while(str[i] == '\n' || str[i] == '\r')
    {
        str.erase(i);
        i--;
    }
}


bool Server::parce_nick(std::string nick)
{
    int i = 0;
    while (nick[i])
    {
        if (i == 0 && !std::isalpha(nick[i]))
            return false;
        if (std::isspace(nick[i]))
            return false;
        else if (nick[i] != '[' && nick[i] != ']' && nick[i] != '\\' && nick[i] != '{' && nick[i] != '}' && nick[i] != '|' && !std::isalnum(nick[i]))
            return false;
        i++;
    }
    return true;
}


void Server::nickName(int fd, std::string data)
{
    std::string nick = data.substr(5, data.size() - 5);
    std::vector<Client>::iterator it = get_client(fd);
    rmoveNew_line(nick);
    if (!it->get_hasPass()) 
    {
        send_msg(ERR_NOTREGISTERED,fd);
        return;
    }
    if (!parce_nick(nick))
    {
        send_msg(ERR_ERRONEUSNICKNAME(nick), fd);
        return;
    }
    for (std::vector<Client>::iterator client = __clients.begin(); client != __clients.end(); ++client)
    {
        
        if (client->get_nickName() == nick)
        {
            send_msg(ERR_NICKNAMEINUSE(nick), fd);
            return;
        }
    }
    if (it != __clients.end())
    {
        it->set_nickName(nick);
        it->set_hasNick(true);
    }
}


void Server::passWord(int fd, std::string data)
{
    std::string pass = data.substr(5, data.size() - 5);
    std::vector<Client>::iterator it = get_client(fd);
    rmoveNew_line(pass);
    if (pass == __passWord)
    {
        it->set_hasPass(true);
        it->set_IpAdd(getclient_IPadd(fd));
    }
    else
        send_msg(ERR_PASSWDMISMATCH(it->get_nickName()), fd);

}

std::vector<std::string> Server::split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
        result.push_back(token);
    return result;
}

void Server::send_msg(std::string msg, int fd)
{
    send(fd, msg.c_str(), msg.size(), 0);
}

bool Server::setPort(std::string str, int &port)
{
    std::stringstream ss(str);

    ss >> port;
    return ((ss.eof() && !ss.fail()) && !(port < 1024 || port > 65535));
}

bool Server::check_Passowrd(std::string password)
{
    if (password.empty())
        return false;
    for (int i = 0; i < static_cast<int>(password.size()); i++)
    {
        if (std::isspace(password[i]))
            return false;
    }
    return true;
}

Channel *Server::getChannel(std::string name)
{
	for (size_t i = 0; i < __channels.size(); i++)
	{
		if (name == __channels[i].get_name())
			return &__channels[i];
	}
	return NULL;
}
Client *Server::getClient(std::string name)
{
	for (size_t i = 0; i < __clients.size(); i++)
	{
		if (name == __clients[i].get_nickName())
			return &__clients[i];
	}
	return NULL;
}

std::string Server::getclient_IPadd(int fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char client_IPadd[INET_ADDRSTRLEN];

    if (getpeername(fd, (struct sockaddr *)&client_addr, &client_len) == -1)
		throw std::runtime_error("ERROR: getpeername failed");
    if (inet_ntop(AF_INET, &client_addr.sin_addr, client_IPadd, sizeof(client_IPadd)) == NULL)
		throw std::runtime_error("ERROR: inet_ntop failed");
    return std::string(client_IPadd);
}