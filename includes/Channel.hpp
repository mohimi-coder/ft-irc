/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:06:32 by hbettal           #+#    #+#             */
/*   Updated: 2025/03/16 13:21:38 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>


class Channel
{
    private:
        std::string             key;
        std::string             name;
        std::vector<Client>     admins;
        std::vector<Client>     members;
        std::vector<Client>     invited;
        std::string             topic;
        bool                    invOnly;
		bool					topicMode;
        size_t                  limits;

    public:

        Channel( void );
        Channel(std::string name);

        //getters
        std::string				get_key();
        std::string				get_name();
        std::vector<Client>		get_admins();
		std::string				get_topic();
        std::vector<Client>&	getMembers();
        size_t                  getlimits();
		bool 					getInvOnlyMode();
		bool 					getTopicMode();

        //setters
        void	set_key(std::string key);
		void	set_topic(std::string topic);
        void	set_admin(Client user);
        void    set_limits(size_t num);
		void	set_inviteMode(bool mode);
		void	set_topicMode(bool mode);

        void		addNewMember(Client user);
        void        inviteClient(Client user);
        void        kickMember(Client user);
        void        removeAdmin(Client user);
        bool		memberExist(Client user);
		bool		isAdmine(Client user);
		bool		isInvited(Client user);
        std::string nameReply();
        static void	sendWelcomeMsg(Client user, Channel room);
        static void sendModeMsg(Client user, Channel room, std::string mode, std::string param);
        static void sendKickingMsg(Client sender, Channel room, Client target, std::string comment);
};
#endif