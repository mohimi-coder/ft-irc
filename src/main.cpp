/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohimi <mohimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:42:49 by mohimi            #+#    #+#             */
/*   Updated: 2025/03/18 01:18:03 by mohimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int main(int ac, char **av)
{
    signal(SIGINT, Server::shutdown_sig);
    signal(SIGQUIT, Server::shutdown_sig);
	int			port;
	std::string	password;
	if (ac != 3 || !Server::setPort(av[1], port) || !Server::check_Passowrd(av[2]))
	{
		std::cerr << "Error: invalid arg <port (1024 <->65535) password>" << std::endl;
		return (1);
	}
	Server s(port, av[2]);
	try
	{
		s.Server_Launcher();
	} catch (std::exception &e)
	{
        s.clearAll_Fds();
		std::cout << e.what() << std::endl;
		return (1);
	}
}