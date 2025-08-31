NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

SRCS = src/utils/card_list.c \
	src/environment/environment.c \
	src/expansion/expand.c \
	src/lexer/lexer.c \
	src/parser/parser_utils.c \
	src/parser/parser.c \
	src/executor/exec_utils.c \
	src/executor/exec_parts.c \
	src/executor/exec_redirect.c \
	src/executor/exec_heredoc.c \
	src/executor/exec_child.c \
	src/executor/exec_path.c \
	src/executor/exec_env.c \
	src/executor/exec_fork.c \
	src/executor/exec_pipeline.c \
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
	src/signal/signal_utils.c \
	src/lexer/lexer_utils.c \
	src/expansion/expand_utils.c \
	src/expansion/node_utils.c \
	src/expansion/quote_ignore.c \
	src/utils/libft_utils.c \
	src/utils/ft_split.c \
	src/utils/libft_utils2.c \
	src/main.c \
	src/utils/clean.c \
	src/expansion/expand_check.c \
	src/executor/exec_heredoc_utils.c \

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