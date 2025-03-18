/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:41:11 by hbettal           #+#    #+#             */
/*   Updated: 2025/03/17 12:42:48 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <sys/_types/_ssize_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>

#define NICK "bot"

void sendMessage(int sock, const std::string message);
void handleResponse(int sock, std::string response);
