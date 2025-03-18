/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:59:40 by hbettal           #+#    #+#             */
/*   Updated: 2025/03/17 14:49:35 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/bot.hpp"

void sendMessage(int sock, const std::string message)
{
    std::string msg = message + "\r\n";
    send(sock, msg.c_str(), msg.length(), 0);
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
        result.push_back(token);
    return result;
}

void rmoveNew_line(std::string &str)
{
    std::string s = str;
    int i = str.size() - 1;
    while(str[i] == '\n' || str[i] == '\r')
    {
        str.erase(i);
        i--;
    }
}

std::string getRandDikr()
{
    std::string adkar[] =
    {
        "استغفر الله",
        "استغفر الله العظيم",
        "استغفر الله وأتوب إليه",
        "اللهم اغفر لي",
        "رب اغفر لي وتب علي إنك أنت التواب الرحيم",
        "اللهم إني ظلمت نفسي فاغفر لي",
        "أستغفر الله الذي لا إله إلا هو الحي القيوم وأتوب إليه",
        "اللهم اجعلني من التوابين واجعلني من المتطهرين",
        "اللهم اغفر لي ذنوبي كلها دقها وجلها، أولها وآخرها، سرها وعلانيتها",
        "اللهم إنك عفو تحب العفو فاعف عني",
        "اللهم صل وسلم وبارك على نبينا محمد",
        "اللهم صل على محمد وعلى آل محمد كما صليت على إبراهيم وعلى آل إبراهيم إنك حميد مجيد",
        "اللهم بارك على محمد وعلى آل محمد كما باركت على إبراهيم وعلى آل إبراهيم إنك حميد مجيد",
        "اللهم صل وسلم على سيدنا محمد عدد ما ذكره الذاكرون",
        "اللهم صل وسلم على نبينا محمد في الأولين والآخرين"
    };
    return adkar[std::rand() % 10];
}

std::string getRandQuote()
{
    std::string quots[] =
    {
        "\"The only way to do great work is to love what you do.\" - Steve Jobs",
        "\"Success is not final, failure is not fatal: it is the courage to continue that counts.\" - Winston Churchill",
        "\"Believe you can and you're halfway there.\" - Theodore Roosevelt",
        "\"It does not matter how slowly you go as long as you do not stop.\" - Confucius",
        "\"Opportunities don't happen. You create them.\" - Chris Grosser",
        "\"The secret of getting ahead is getting started.\" - Mark Twain",
        "\"Difficulties in life are intended to make us better, not bitter.\" - Dan Reeves",
        "\"Do what you can, with what you have, where you are.\" - Theodore Roosevelt",
        "\"Hardships often prepare ordinary people for an extraordinary destiny.\" - C.S. Lewis",
        "\"Don't watch the clock; do what it does. Keep going.\" - Sam Levenson"
    };
    return quots[std::rand() % 10];
}

void handleResponse(int sock, std::string response)
{
    if (response.find("PRIVMSG " + std::string(NICK)) == std::string::npos)
        return;
    std::vector<std::string> msg = split(response, ':');
    if (msg.empty() || msg.size() < 2)
        return;
    std::vector<std::string> tmp = split(msg[1], '!');
    if (tmp.empty())
        return;
    std::string clientName = tmp[0];
    std::vector<std::string> names = split(msg[msg.size() - 1], ' ');
    std::string command = names[0];
    names.erase(names.begin());
    std::srand(time(0));
    rmoveNew_line(command);
    if (command == "DIKR")
    {
        sendMessage(sock, "PRIVMSG " + clientName + " : " + getRandDikr());
        return;
    }
    else if (command == "QUOTE")
    {
        sendMessage(sock, "PRIVMSG " + clientName + " : " + getRandQuote());
        return;
    }
    else if (command == "RAND")
    {
        if (names.size() < 2)
        {
            sendMessage(sock, "PRIVMSG " + clientName + " : Invalid number of arguments");
            return ;
        }
        std::string winner = names[std::rand() % names.size()];
        sendMessage(sock, "PRIVMSG " + clientName + " : ===> " + winner + " <=== is the chosen one");
    }
    else if (command == "PING")
    {
        sendMessage(sock, "PRIVMSG " + clientName + " : PONG");
        return;
    }
    else if (command == "HELP")
    {
        sendMessage(sock, "PRIVMSG " + clientName + " : BOT commands: DIKR : get a dikr, RAND : choose a random winner, PING : check if the bot is alive, QUOTE : get a motivation quote");
        return;
    }
    else
    {
        sendMessage(sock, "PRIVMSG " + clientName + " : Invalid BOT command (DIKR, RAND, PING, QUOTE, HELP)");
        return;
    }
}
