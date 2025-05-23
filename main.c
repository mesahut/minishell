
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

typedef struct s_env
{
    char *key;
    char *value;
    void *next;
} t_env;



void create_card(t_env **node, char *src)
{
    t_env  *new_node;
    t_env  *current;

    new_node = (t_env *) malloc(sizeof(t_env));
    if(!new_node)
        ();
    
    new_node->next = NULL;
    if(*node == NULL)
        *node = new_node;
    else
        current = *node;
        while (current->next != NULL)
            current = current->next;
        current->next = new_node;
}

int quote_checker(char *words)
{
    int i;
    char    open_quote;

    i = 0;
    open_quote = '\0';
    while (words[i])
    {
        if(open_quote == '\0' && (words[i] == 34 || words[i] == 39))
            open_quote = words[i];
        else if(open_quote == words[i])
            open_quote = '\0';
        i++;

    }
    return (open_quote = '\0'); // buraya bak
}

void    is_quoted(char *arg)
{
    int     i;
    char    open_quote;

    i = 0;
    open_quote = '\0';
    while (arg[i])
    {
        if (arg[i] == 39 || arg[i] == 34)
            open_quote = arg[i];
        if(open_quote == 34)
            i = quote_ignore(arg, &open_quote);
        else if(open_quote == 39)
            i = quote_ignore(arg, &open_quote);
        else
            i++;

    }
    
}
int quote_ignore(char *arg, int *open_quote)
{
    
}

void    lexer(char *line)
{
    char    **words;
    int     i;

    i = 0;
    quote_checker(line);
    words = ft_split(line, ' ');
    while (words[i])
    {
        is_quoted(words[i]);
        i++;
    }
    return;
}

int main(int argc, char **argv, char **env)
{
    char *line;
    while (1)
    {
        line = readline("minishell>>");
        add_history(line);
        lexer(line);
        //expander(a);
/*         while (a[i])
            printf("%s\n", a[i++]);
        i = 0;
        while (a[i])
            free(a[i++]); */
    }
    return 0;
}
