C++ = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 
SRC = src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp src/Join.cpp src/topic.cpp src/Invite.cpp src/kick.cpp src/privmsg.cpp src/mode.cpp
BONUSSRC = bonus/bot.cpp bonus/main.cpp
NAME = ircserv
BONUSNAME = bot
OBJCS = $(SRC:.cpp=.o)
BONUSOBJCS = $(BONUSSRC:.cpp=.o)

all: $(NAME)

bonus: $(BONUSNAME)

$(NAME): $(OBJCS)
		$(C++) $(FLAGS) -o $(NAME) $(OBJCS)

$(BONUSNAME): $(BONUSOBJCS)
		$(C++) $(FLAGS) -o $(BONUSNAME) $(BONUSOBJCS)

%.o: %.cpp includes/Server.hpp includes/Client.hpp includes/Channel.hpp includes/Replies.hpp
	$(C++) $(FLAGS) -c $< -o $@ 

%bonus/.o: %bonus/.cpp bot.hpp
	$(C++) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJCS) $(BONUSOBJCS)

fclean: clean
	rm -f $(NAME) $(BONUSNAME)

re: fclean all

.PHONY: clean