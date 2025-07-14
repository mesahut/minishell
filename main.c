#include "lexer.h"

/*void    is_quoted(char *arg)
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
    
}*/
void free_list(t_list *list)
{
    t_list *current;

    while (list)
    {
        current = list;
        list = list->next;
        free(current->value);
        free(current);
    }
}

void free_all(t_all *all)
{
    free_list(t_all->collector);
    free_list(t_all->card);
    free_list(t_all->env);
    return (1);
}

int main(int argc, char **argv, char **env_list)
{
    t_card  *card;
    char    *line;
    t_env   *env;
    t_card  *node = NULL;

    card = NULL;
    env = NULL;
    put_env(&env, env_list);
    while (1)
    {
        line = readline("minishell>>");
        add_history(line);
        lexer(line, &card);
        expander(&card);
        //node = card;
        //while(node != NULL)
        //{
        //    printf("%s\n", node->value);
        //    node = node->next;
        //}
    }
    return 0;
}
