
#ifndef LEXER_H

#define LEXER_h

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

#define PIPE 0
#define R_APPEND 1
#define R_OUT 2
#define HEREDOC 3
#define R_IN 4
#define WORD 5

typedef struct s_list
{
    void *value;
    struct s_list *next;
} t_list;


typedef struct s_card 
{
    int     type;
    char    *value;
    struct s_card *next;
} t_card;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_card *s_env;
} t_env;

void    create_card(t_card **head_card, char *card);
void    mid_card(t_card **current_node, char *str, int type);
void    expander(t_card **card);
void    lexer(char *line, t_card **card);
void    put_env(t_env **env, char **env_list);

#endif