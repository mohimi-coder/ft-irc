/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohimi <mohimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 22:49:46 by mohimi            #+#    #+#             */
/*   Updated: 2025/03/18 00:55:14 by mohimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <sstream>
#include <exception>
#include <cstring>


#include "Channel.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define color "\001\033[1;36m\002"
#define pos "\001\033[0;0m\002"
#define yellow "\001\033[1;33m\002"
#define grey "\001\033[1;30m\002"
#define lg "\001\033[92m\002"
#define b_italic "\001\033[1;3m\002"
#define gold "\001\033[38;5;220m\002" 
#define dark_blue "\001\033[34m\002"
#define teal "\001\033[38;5;30m\002"
#define violet "\001\033[38;5;128m\002"
#define b_ora "\001\033[1;38;5;208m\002"
#define RED "\x1b[31m"
#define ORANGE "\x1b[38;5;214m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"


class Server
{
    private :
        int                                __port;
        std::string                        __passWord;
        int                                __fd_socket;
        static   bool                      __signal;
        std::vector<struct pollfd>         __fDs;
        std::vector<Client>                __clients;
        std::vector<Channel>                __channels;
        Server(const Server &src);
        Server &operator=(const Server &src);
    public :
        Server(int port, std::string pass_word);

		//getters

		Channel *getChannel(std::string name);
		Client  *getClient(std::string name);
		
        void                            Server_Launcher();
        void                            ServerSocket();
        void                            addNew_Client();
        int                             get_Fdsocket();
        std::vector<Client>::iterator   get_client(int fd);
        void                            ReceiveNewData(int fd);
        void                            clearAll_Fds();
        void                            userName(int fd, std::string data);
        void                            nickName(int fd, std::string data);
        void                            passWord(int fd, std::string data);
         std::vector<Client>::iterator  client_nick(std::string nick_name);
        static std::vector<std::string> split(const std::string &str, char delimiter);
        static void                     shutdown_sig(int signal);
        static void                     send_msg(std::string msg, int fd);
        void                            rmoveNew_line(std::string &str);
        bool                            parce_nick(std::string nick);
        static bool                     setPort(std::string str, int &port);
        static bool                     check_Passowrd(std::string password);
        void                            handleCommands(int fd, std::string &data, Client *client);
        std::string                     getclient_IPadd(int fd);

        //channel

        void                            join(int fd, std::string data, Client *_user);
		void							topic(std::string data, Client *user);
		void							invite(std::string data, Client user);
        void                            kick(std::string data, Client user);
		void							mode(std::string data, Client user);
		void						    privmsg(std::string data, Client user);
        void                            leaveChannels(Client *user);
		void							part(std::string data, Client client);
        ~Server();
}; 

#endif