NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
INCLUDE = -I. -I./libft
LIBFT = libft/libft.a

SRCS = src/main.c \
	src/utils/card_list.c \
	src/environment/environment.c \
	src/expansion/expand.c \
	src/lexer/lexer.c \
	src/parser/parser.c \
	src/executor/exec.c \
	src/executor/exec_utils.c \
	src/executor/exec_parts.c \
	src/builtins/builtin.c \
	src/builtins/cd.c \
	src/builtins/pwd.c \
	src/builtins/env_print.c \
	src/builtins/echo.c \
	src/builtins/env.c \
	src/builtins/unset.c \
	src/builtins/export.c \
	src/builtins/exit.c \
	src/utils/utils.c \
	src/signal/signal.c \
	src/executor/exec_clean.c \
	src/lexer/lexer_utils.c \
	src/expansion/expand_utils.c \
	src/expansion/node_utils.c \
	src/expansion/quote_ignore.c 
	
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