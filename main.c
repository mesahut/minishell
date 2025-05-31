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

int main(int argc, char **argv, char **env_list)
{
    t_card  *card;
    char    *line;
    t_env   *env;

    card = NULL;
    env = NULL;
    while (1)
    {
        line = readline("minishell>>");
        add_history(line);
        lexer(line, &card);
       expender(&card, &env, env_list);
        //printf("%s\n", env_list[0]);
//        while(card != NULL)
//        {
//            printf("%s\n", card->value);
//            card = card->next;
//        }
        
        //expander(a);
/*         while (a[i])
            printf("%s\n", a[i++]);
        i = 0;
        while (a[i])
            free(a[i++]); */
    }
    return 0;
}
