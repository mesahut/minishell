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

int main(int argc, char **argv, char **env_list)
{
    t_all   *all;
    char    *line;
    t_card  *card = NULL;
    char    *input;

    all = (t_all *)malloc(sizeof(t_all));
    if (!all)
        return 1;
    all->exit_status = 0;
    put_env(&(all->env) , env_list);
    while (1)
    {
        line = readline("minishell>>");
        add_history(line);
        line = collector_dup(all->collector, line);
        input = line;
        lexer(input, all);
        expander(all);
        parser(all);
        exec(all);
        card = all->card;
        while(card != NULL)
        {
            printf("%s\n", card->value);
            card = card->next;
        }
    }
    return 0;
}
