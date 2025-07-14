#include <stdio.h>
#include "lexer.h"


int main(int argc, char **argv, char **env_list)
{
    t_card  *card;
    char    *line;
    t_env   *env;
    t_card  *node = NULL;

    card = NULL;
    env = NULL;
    while (1)
    {
        line = readline("minishell>>");
        add_history(line);
        lexer(line, &card);
        node = card;
        while(node != NULL)
        {
            printf("%s\n", node->value);
            node = node->next;
        }
    }
    return 0;
}