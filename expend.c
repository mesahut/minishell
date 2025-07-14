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

char	*found_dollar(char *line, int dollar_place)
{
	char	*before;
	char	*after;
	char	*env;
	int		len;

    //  if(line[dollar_place + 1] == '?')
    //     handle_exit_status();
	len = 0;
	before = ft_substr(line, 0, dollar_place); // freelenecek
	while(line[dollar_place + len + 1] != '\0' && (ft_isalnum(line[dollar_place + len + 1]) == 1))
		len++;
	env = ft_substr(line, dollar_place + 1, len); // freelenecek
	env = getenv(env);
	after = ft_substr(line, dollar_place + len + 1, ft_strlen(line) - (dollar_place + len + 1)); //
	before = ft_strjoin(before, env);
	after = ft_strjoin(before, after);
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

void	check_for_expansion(t_card **cards)
{
	int		i;
	t_card	*node;
	char	open_quote;

	i = 0;
	open_quote = '\0';
	node = (*cards);
	while (node != NULL)
	{
		i = 0;
		while ((node->value)[i])
		{
			open_quote = is_char_quote((node->value)[i], open_quote);
			if(open_quote != '\'' && (node->value)[i] == '$')
			{
				node->value = found_dollar((node->value), i);
			}
			i++;
		}
		node = node->next;
	}
}

void    tokenize_operator(t_card **card,int o_place, int type)
{
	char	*after;
	char	*operator;

	if(type == R_APPEND || type == HEREDOC)
	{
		after = ft_substr((*card)->value, o_place + 2, ft_strlen((*card)->value) - (o_place + 2));
		operator = ft_substr((*card)->value, o_place, 2);
	}
	else
	{
		after = ft_substr((*card)->value, o_place + 1, ft_strlen((*card)->value) - (o_place + 1));
		operator = ft_substr((*card)->value, o_place, 1);
	}
	
	mid_card();
}

void	operator_searcher(t_card **card)
{
	int	i;

	i = 0;
	while((*card)->value[i])
	{
		if ((*card)->value[i] == "|")
			tokenize_operator(card, i, PIPE);
		else if ((*card)->value[i] == "<" && (*card)->value[i + 1] == "<")
			tokenize_operator(card, i, HEREDOC);
		else if ((*card)->value[i] == "<")
			tokenize_operator(card, i, R_IN);
		else if ((*card)->value[i] == ">" && (*card)->value[i + 1] == ">>")
			tokenize_operator(card, i, R_APPEND);
		else if ((*card)->value[i] == ">")
			tokenize_operator(R_OUT);
		i++;
	}
}
void	put_title(t_card **card)
{
	t_card	*current;

	current = (*card);
	while (current)
	{
		operator_searcher(current);
		current = current->next;
	}
}

void	expander(t_card **card)
{
	check_for_expansion(card);
	put_title(card);
}
