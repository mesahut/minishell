NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

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
	src/expansion/quote_ignore.c \
	src/utils/libft_utils.c \
	src/utils/ft_split.c \
	
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re