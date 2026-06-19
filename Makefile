NAME		:= webserv

CC			:= c++
CFLAGS		:= -Wall -Wextra -Werror -g -O0 -std=c++98 -pedantic
RM			:= rm -rf

INC_DIR		:= includes
SRC_DIR		:= sources
OBJ_DIR		:= objects

SOCKET_DIR	:= sockets
SOCKET_PREFIX:= $(SRC_DIR)/$(SOCKET_DIR)/

ROOT_SRC	:= main.cpp support.cpp
SOCKET_SRC	:= create_listening_socket.cpp manage_clients.cpp support.cpp epoll.cpp

SRCS		:= $(addprefix $(SRC_DIR)/, $(ROOT_SRC)) \
			   $(addprefix $(SOCKET_PREFIX), $(SOCKET_SRC))

OBJS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

INC			:= -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $@
	echo "$(NAME) created successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	echo "Cleaning object files..."
	$(RM) $(OBJ_DIR)
	echo "Object files cleaned."

fclean: clean
	echo "Cleaning executable and library..."
	$(RM) $(NAME)
	echo "Executable and library cleaned."

re: fclean all

.SILENT:
.PHONY: all clean fclean re
