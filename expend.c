/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:36:24 by marvin            #+#    #+#             */
/*   Updated: 2025/05/29 16:36:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void    create_env(char **env_line, t_env **env)
{
    t_env   *new_env;
    t_env   *current;

    new_env = (t_env *)malloc(sizeof(t_env));

    new_env->key = env_line[0];
    new_env->value = env_line[1];
    new_env->next = NULL;
    if(*env == NULL)
        *env = new_env;
    else
    {
        current = *env;
        while (current->next != NULL)
            current = current->next;
        current->next = new_env;
    }
}

void    put_env(t_env **env, char **env_list)
{
    int i;

    i = 0;
    while (env_list[i])
    {
        create_env(ft_split(env_list[i], '='), env);
        i++;
    }
    
}

//void    found_tilde(char *line,int tilde_place)
//{
//    int     len;
//    char    *expend;
//    char    *before;
//    char    *after;
//
//    len = ft_strlen(line);
//    before = ft_substr(line, 0, tilde_place - 1);
//    expend = getenv("HOME");
//    after = ft_substr(line, tilde_place + 1, );
//    
//}
char    *found_dollar(char *line, int dollar_place)
{
    char    *expend;
    char    *before;
    char    *after;
    char    *result;
    int     len;

    len = 0;
    before = ft_substr(line, 0, dollar_place);
    while(line[dollar_place + len] != '\0' && (ft_isalnum(dollar_place + len) == 1))
        len++;
    expend = getenv(ft_substr(line, dollar_place + 1, len));
    after = ft_substr(line, dollar_place + len, ft_strlen(line) - (dollar_place + len));
    result = ft_strjoin(ft_strjoin(before, expend), after);
    return(result);
}

void check_for_expansion(t_card **cards)
{
    int i;
    t_card  *node;
    char    open_quote;

    i = 0;
    open_quote = '\0';
    node = (*cards);
    while (node != NULL)
    {
        while ((node->value)[i])
        {
            open_quote = is_char_quote((node->value)[i]);
            if(open_quote != '\'' && (node->value)[i] == '$')
                found_dollar((node->value), i);
            if()
        }
    }
    
}

void    expender(t_card **card, t_env **env, char **env_list)
{
    put_env(env, env_list);
    check_for_expansion(card);

}
