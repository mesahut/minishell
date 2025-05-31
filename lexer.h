
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
#define U_WORD 5
#define S_WORD 6
#define D_WORD 7

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
    struct s_env *next;
} t_env;

void    create_card(t_card **head_card, char *card);
void    expender(t_card **card, t_env **env, char **env_list);
void    lexer(char *line, t_card **card);

#endif