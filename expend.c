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
#include <stdlib.h>
#include <string.h>

char	*found_dollar(char *line, int dollar_place, t_collector *collector)
{
	char	*before;
	char	*after;
	char	*env;
	int		len;

	len = 0;
	before = ft_substr(line, 0, dollar_place);
	if(line[dollar_place + 1] == '?')
		len = 2;
	else
		while(line[dollar_place + len + 1] != '\0' && (ft_isalnum(line[dollar_place + len + 1]) == 1))
			len++;
	after = ft_substr(line, dollar_place + 1, len);
	env = getenv(after);
	free(after);
	after = ft_substr(line, dollar_place + len + 1, ft_strlen(line) - (dollar_place + len + 1));
	before = ft_strjoin(before, env);
	after = ft_strjoin(before, after);
	after = collector_dup(collector, after);
	return(after);
}

char	is_char_quote(char value, char quote_type)
{
	if (value == '\'' || value == '"')
	{
		if(value == '\'')
		{
			if (quote_type == '\0')
				quote_type = '\'';
			else if(quote_type == '\'')
				quote_type = '\0';
		}
		else
		{
			if(quote_type == '\0')
				quote_type = '"';
			else if(quote_type == '"')
				quote_type = '\0';
		}
	}
	return(quote_type);
}

void	check_for_expansion(t_all *all)
{
	int		i;
	t_card	*node;
	t_card	*prev_node;
	char	open_quote;

	i = 0;
	open_quote = '\0';
	node = all->card;
	prev_node = node;
	prev_node->type = -1;
	while (node != NULL)
	{
		i = 0;
		while ((node->value)[i])
		{
			open_quote = is_char_quote((node->value)[i], open_quote);
			if(open_quote != '\'' && prev_node->type != HEREDOC && (node->value)[i] == '$')
			{
				node->value = found_dollar((node->value), i, all->collector);
			}
			i++;
		}
		prev_node = node;
		node = node->next;
	}
}

void	put_title(t_all *all)
{
	t_card	*current;

	current = all->card;
	while (current)
	{
		if (current->value[0] == '|')
			current->type = PIPE;
		else if (current->value[0] == '<' && current->value[1] == '<')
			current->type = HEREDOC;
		else if (current->value[0] == '<')
			current->type = R_IN;
		else if (current->value[0] == '>' && current->value[1] == '>')
			current->type = R_APPEND;
		else if (current->value[0] == '>')
			current->type = R_OUT;
		else
			current->type = WORD;
		current = current->next;
	}
}
void	quote_select(char *str)
{
	    while (str[i])
    {
        if ((str[i] == '\'' && open_quote != '"') ||
            (str[i] == '"' && open_quote != '\''))
        {
            if (open_quote == '\0')
                open_quote = str[i];
            else if (open_quote == str[i])
                open_quote = '\0';
            count++;
        }
        i++;
    }
}

int quote_count(char *str)
{
    int i = 0;
    int count = 0;
    char open_quote = '\0';
    while (str[i])
    {
        if ((str[i] == '\'' && open_quote != '"') ||
            (str[i] == '"' && open_quote != '\''))
        {
            if (open_quote == '\0')
                open_quote = str[i];
            else if (open_quote == str[i])
                open_quote = '\0';
            count++;
        }
        i++;
    }
    return count;
}

char *quote_ignore(char *str)
{
    int len = strlen(str);
    int quotes = quote_count(str);
    char *result = (char *)malloc(len - quotes + 1);
    int i = 0, j = 0;
    char open_quote = '\0';
    while (str[i])
    {
        if ((str[i] == '\'' && open_quote != '"') ||
            (str[i] == '"' && open_quote != '\''))
        {
            if (open_quote == '\0')
                open_quote = str[i];
            else if (open_quote == str[i])
                open_quote = '\0';
            // Tırnak karakterini atla
            i++;
            continue;
        }
        result[j++] = str[i++];
    }
    result[j] = '\0';
    return result;
}

void	quote_ingnore(t_all *all)
{
	t_card	*current;

	current = all->card;
	while (current)
	{
		current->value = quote_ignore(current->value);
		current = current->next;
	}
}

void	expander(t_all *all)
{
	put_title(all);
	check_for_expansion(all);
	quote_ingnore(all);
}
