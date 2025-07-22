#ifndef LEXER_H

#define LEXER_H

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

typedef struct s_collector
{
    void *value;
    struct s_collector *next;
} t_collector;

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

typedef struct s_redirect
{
    int     type;
    char    *filename;
    char    *value;
    int     fd;
    struct s_redirect *next;
} t_redirect;

typedef struct s_cmd
{
    char **args;
    int fd_in;
    int fd_out;
    int args_count;
    int redirect_count;
    t_redirect *redirects;
    struct s_cmd *next;
    struct s_cmd *prev;
} t_cmd;

typedef struct s_all
{
    t_collector *collector;
    t_card      *card;
    t_cmd       *cmd;
    t_env       *env;
    int         exit_status;
} t_all;

typedef int (*t_builtin_func)(t_all *all, t_cmd *cmd);

typedef struct s_builtin
{
    char *name;
    t_builtin_func func;
} t_builtin;

void    set_cmd(t_all *all, t_cmd *cmd);
void    parser(t_all *all);
void    exec(t_all *all);
void    create_card(t_all *all, char *card);
void	expander(t_all *all);
void    lexer(char *line, t_all *all);
void    put_env(t_env **env, char **env_list);
void    *safe_malloc(t_collector *gc_head, int size);
char    *collector_dup(t_collector *collector, char *line);
int     exec_builtin(t_all *all, t_cmd *cmd);
int     is_builtin(char *cmd);
void print_env(t_env *env);
#endif