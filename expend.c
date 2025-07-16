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
	char	open_quote;

	i = 0;
	open_quote = '\0';
	node = all->card;
	while (node != NULL)
	{
		i = 0;
		while ((node->value)[i])
		{
			open_quote = is_char_quote((node->value)[i], open_quote);
			if(open_quote != '\'' && (node->value)[i] == '$')
			{
				node->value = found_dollar((node->value), i, all->collector);
			}
			i++;
		}
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

void	expander(t_all *all)
{
	check_for_expansion(all);
	put_title(all);
}
