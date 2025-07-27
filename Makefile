NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline
INCLUDE = -I. -I./libft
LIBFT = libft/libft.a

SRCS = src/main.c \
	src/utils/card_list.c \
	src/environment/environment.c \
	src/expansion/expend.c \
	src/lexer/lexer.c \
	src/parser/parser.c \
	src/executor/exec.c \
	src/builtins/builtin.c 

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C ./libft

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(MAKE) -C ./libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C ./libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re