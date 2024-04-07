### COMPILATION ###

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98

### EXECUTABLE ###

NAME    = ircserv


### INCLUDES ###
OBJ_PATH  = .Objs
HEADER = Includes
SRC_PATH  = .

### SOURCE FILES ###

SOURCES =	Sources/main.cpp \
			Sources/Server.cpp \
			Sources/Client.cpp \
			Sources/Channel.cpp \
			Commands/Utils.cpp \
			Commands/Join.cpp \
			Commands/Pass.cpp \
			Commands/Nick.cpp \
			Commands/User.cpp \
			Commands/Privmsg.cpp \
			Commands/Part.cpp \
			Commands/Kick.cpp \
			Commands/Motd.cpp \
			Commands/List.cpp \
			Commands/Ping.cpp \
			Commands/Quit.cpp \
			Commands/Invite.cpp \
			Commands/Topic.cpp \
			Commands/Lusers.cpp \
			Commands/Mode.cpp \
			Commands/Mode_L.cpp \
			Commands/Mode_O.cpp \
			Commands/Mode_I.cpp \
			Commands/Mode_T.cpp \
			Commands/Mode_K.cpp \
			Sources/Bot.cpp \
			
SRCS 	= $(addprefix $(SRC_PATH)/,$(SOURCES))
OBJS    = $(addprefix $(OBJ_PATH)/,$(SOURCES:.cpp=.o))

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^
	@echo "$(GREEN)Project successfully compiled\n"


$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp $(HEADER)/*.hpp Commands/Commands.hpp Makefile
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c -o $@ $< 
	@echo "$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]$(NOC)"

clean:
	@echo "$(GREEN)Supressing libraries files$(CYAN)"
	@rm -rf $(OBJ_PATH)

fclean:
	@echo "$(GREEN)Supressing libraries files$(CYAN)"
	@rm -rf $(OBJ_PATH)
	@rm -f $(NAME)

re: fclean
	@$(MAKE) all


### COLORS ###
NOC         = \033[0m
BOLD        = \033[1m
UNDERLINE   = \033[4m
BLACK       = \033[1;30m
RED         = \033[1;31m
GREEN       = \033[1;32m
YELLOW      = \033[1;33m
BLUE        = \033[1;34m
VIOLET      = \033[1;35m
CYAN        = \033[1;36m
WHITE       = \033[1;37m


.PHONY: all clean fclean re template
