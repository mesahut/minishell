/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_ignore.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 00:30:44 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/09/01 16:20:55 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	is_char_quote(char value, char quote_type)
{
	if (value == '\'' || value == '"')
	{
		if (value == '\'')
		{
			if (quote_type == '\0')
				quote_type = '\'';
			else if (quote_type == '\'')
				quote_type = '\0';
		}
		else
		{
			if (quote_type == '\0')
				quote_type = '"';
			else if (quote_type == '"')
				quote_type = '\0';
		}
	}
	return (quote_type);
}

int	quote_count(char *str)
{
	int		i;
	int		count;
	char	open_quote;

	open_quote = '\0';
	i = 0;
	count = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && open_quote != '"')
			|| (str[i] == '"' && open_quote != '\''))
		{
			if (open_quote == '\0')
				open_quote = str[i];
			else if (open_quote == str[i])
				open_quote = '\0';
			count++;
		}
		i++;
	}
	return (count);
}

int	skip_quotes(char *str, char *open_quote, int i)
{
	if ((str[i] == '\'' && (*open_quote) != '"')
		|| (str[i] == '"' && (*open_quote) != '\''))
	{
		if ((*open_quote) == '\0')
			(*open_quote) = str[i];
		else if ((*open_quote) == str[i])
			(*open_quote) = '\0';
		return (1);
	}
	return (0);
}

char	*quote_ignore(t_all *all, char *str, int quotes)
{
	int		len;
	int		j;
	int		i;
	char	open_quote;
	char	*result;

	i = 0;
	j = 0;
	open_quote = '\0';
	len = ft_strlen(str);
	result = (char *)safe_malloc(all, (len - quotes + 1));
	while (str[i])
	{
		if (skip_quotes(str, &open_quote, i))
		{
			i++;
			continue ;
		}
		result[j] = str[i];
		j++;
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	del_quote(t_all *all)
{
	t_card	*current;
	int		quotes;

	current = all->card;
	while (current)
	{
		quotes = 0;
		quotes = quote_count(current->value);
		current->value = quote_ignore(all, current->value, quotes);
		if (current->type == HEREDOC && current->next
			&& quote_count(current->next->value) != 0)
			current->next->here_flag = 1;
		current = current->next;
	}
}
