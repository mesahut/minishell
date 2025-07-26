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

char	*ft_getenv(t_env *env, char *key)
{
	t_env *current;

	current = env;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return current->value;
		current = current->next;
	}
	return (NULL);
}

void insert_node_at(t_all *all, t_card **pos, char *str)
{
	t_card *tmp;
	t_card *new_node = (t_card *)safe_malloc(all->collector, sizeof(t_card));
	if (!new_node)
		return;
	new_node->value = str;
	new_node->type = WORD;
	new_node->here_flag = 0;
	tmp = (*pos)->next;
	(*pos)->next = new_node;
	new_node->next = tmp;
}

void	delim_node(t_all *all, t_card *node)
{
	char	*str;
	char	**temp;
	int		i;

	i = 1;
	str = NULL;
	temp = ft_split(node->value, ' ');
	free(node->value);
	node->value = temp[0];
	if(temp[i] && flag_check(temp[i]))
	{
		insert_node_at(all, &node, ft_strdup(temp[i]));
		node = node->next;
	}
	else if(temp[i])
		str = temp[i];
	i++;
	while (temp[i])
	{
		str = ft_strjoin(str, " ");
		str = ft_strjoin(str, temp[i]);
		i++;
	}
	if (str)
		insert_node_at(all, &node, ft_strdup(str));
}

char	*found_dollar(char *line, int dollar_place, t_all *all)
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
	env = ft_getenv(all->env, after);
	free(after);
	after = ft_substr(line, dollar_place + len + 1, ft_strlen(line) - (dollar_place + len + 1));
	before = ft_strjoin(before, env);
	after = ft_strjoin(before, after);
	after = collector_dup(all->collector, after);
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
	prev_node->type = WORD;
	while (node != NULL)
	{
		i = 0;
		while ((node->value)[i])
		{
			open_quote = is_char_quote((node->value)[i], open_quote);
			if(open_quote != '\'' && prev_node->type != HEREDOC && (node->value)[i] == '$')
			{
				node->value = found_dollar((node->value), i, all);
				delim_node(all, node);
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

char *quote_ignore(t_all *all, char *str)
{
    int len = strlen(str);
    int quotes = quote_count(str);
    char *result = (char *)safe_malloc(all->collector ,len - quotes + 1);
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
		if(current->type == HEREDOC && current->next)
			current->here_flag = 1;
		current->value = quote_ignore(all, current->value);
		current = current->next;
	}
}

void	expander(t_all *all)
{
	put_title(all);
	check_for_expansion(all);
	quote_ingnore(all);
}
