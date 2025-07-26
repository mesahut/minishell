#include "lexer.h"

char    *collector_dup(t_collector *collector, char *line)
{
    char    *str;
    int     i;

    i = 0;
    str = (char *)safe_malloc(collector, ft_strlen(line) + 1);
    while(line[i])
    {
        str[i] = line[i];
        i++;
    }
    str[i] = '\0';
    free(line);
    return (str);
}

void    print_card(t_card *card)
{
    t_card *current = card;
    while (current)
    {
        printf("Card Type: %d, Value: %s\n", current->type, current->value);
        current = current->next;
    }
}

int main(int argc, char **argv, char **env_list)
{
    (void)argc;
    (void)argv; 
    t_all   *all;
    char    *line = NULL;
    char    *input;

    all = (t_all *)ft_calloc(sizeof(t_all), 1);
    if (!all)
        return 1;
    all->exit_status = 0;
    put_env(all , env_list);
    while (1)
    {
        free(line);
        line = readline("minishell>>");
        add_history(line);
        line = collector_dup(all->collector, line);
        input = line;
        lexer(input, all);
        expander(all);
        print_card(all->card);
        parser(all);
        exec(all);
       // print_env(all->env);
    }
    return 0;
}

