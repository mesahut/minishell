CC=gcc
CFLAGS= -Wall -lreadline
SRCS=	card_list.c \
		expend.c \
		lexer.c \
		main.c \
		environment.c

OBJS=$(SRCS:.c=.o)
TARGET=minishell.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) libft.a -o $@ $^

clean:
	del /Q *.o *.exe