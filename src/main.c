/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:52:26 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/03 13:40:57 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

char    *collector_dup(t_collector **collector, char *line)
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

void reset_all(t_all *all)
{
    if (all->collector)
    {
        clean_malloc(all->collector);
        all->collector = NULL;
    }
    all->card = NULL;
    all->cmd = NULL;
}


int main(int argc, char **argv, char **env_list)
{
    (void)argc;
    (void)argv;

    t_all   all;
    char    *line = NULL;
    char    *input;
    
    /*
    sig_init();

    signal(SIGINT, sig_int);
    signal(SIGQUIT, sig_quit);
    */
   all = (t_all){0};
   put_env(&all, env_list);
   while (1)
   {
       line = readline("minishell>>");    
       // Eğer readline boşsa (ör. Ctrl+D), döngüden çık
       add_history(line);
       input = collector_dup(&all.collector, line);
        if (lexer(input, &all) == 0)
            continue; // Eğer lexer başarısızsa, yeni bir satır oku
        expander(&all);
        parser(&all);
        exec(&all);
        reset_all(&all);
        // print_env(all->env);
    }
    rl_clear_history();
    return 0;
}
