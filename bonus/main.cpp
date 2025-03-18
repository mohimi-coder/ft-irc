/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:40:16 by hbettal           #+#    #+#             */
/*   Updated: 2025/03/17 14:22:20 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot.hpp"

int connectToServer(const char *serverHost, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Socket error" << std::endl;
        return -1;
    }

    struct hostent *server = gethostbyname(serverHost);
    if (!server)
    {
        std::cerr << "Host not found" << std::endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    std::memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connect error" << std::endl;
        return -1;
    }
    return sock;
}

int main(int ac, char **av)
{
    if (ac < 3)
    {
        std::cerr << "./bot <password> <port> <serverIP>" << std::endl;
        return 1;
    }
    int sock = connectToServer(av[3], std::atoi(av[2]));
    if (sock < 0)
        return 1;
    sendMessage(sock, "PASS " + std::string(av[1]));
    usleep(100);
    sendMessage(sock, "USER " NICK " 0 * :" NICK);
    usleep(100);
    sendMessage(sock, "NICK " NICK);
    usleep(100);
    
    char buffer[512];
    std::memset(buffer, 0, sizeof(buffer));
    while (true)
    {
        ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
            break;
        buffer[bytes] = '\0';
        if (std::string(buffer).find('\n') != std::string::npos)
            handleResponse(sock, std::string(buffer));
    }
    close(sock);
    return 0;
}