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

#include "../../include/minishell.h"
#include <stdlib.h>
#include <string.h>

char	*ft_getenv(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	check_node(t_card *card, t_card *prev)
{
	if (card->value[0] != '\0')
		return ;
	prev->next = card->next;
	card = prev;
	return ;
}

void	insert_node_at(t_all *all, t_card **pos, char *str)
{
	t_card	*tmp;
	t_card	*new_node;

	new_node = (t_card *)safe_malloc(&all->collector, sizeof(t_card));
	if (!new_node)
		return ;
	new_node->value = str;
	new_node->type = WORD;
	new_node->here_flag = 0;
	tmp = (*pos)->next;
	(*pos)->next = new_node;
	new_node->next = tmp;
}

int	flag_check(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	delim_node(t_all *all, t_card *node)
{
	char	*str;
	char	**temp;
	int		i;

	i = 1;
	str = NULL;
	temp = ft_split(node->value, ' ');
	node->value = collector_dup(&all->collector, temp[0]);
	if (temp[i] && flag_check(temp[i]))
	{
		insert_node_at(all, &node, ft_strdup(temp[i]));
		node = node->next;
		i++;
	}
	else if (temp[i])
	{
		str = temp[i];
		i++;
	}
	while (temp && temp[i])
	{
		str = ft_strjoin(str, " ");
		str = ft_strjoin(str, temp[i]);
		i++;
	}
	if (str)
		insert_node_at(all, &node, ft_strdup(str));
	free_split(temp);
}

char	*found_dollar(char *line, int dollar_place, t_all *all)
{
	char	*before;
	char	*after;
	char	*env;
	int		len;
	
	len = 0;
	before = ft_substr(line, 0, dollar_place);
	if (line[dollar_place + 1] == '?')
		len = 1;
	else
		while (line[dollar_place + len + 1] != '\0' && (ft_isalnum(line[dollar_place + len + 1]) == 1))
			len++;
	after = ft_substr(line, dollar_place + 1, len);
	env = ft_getenv(all->env, after);
	env = ft_strdup(env);
	free(after);
	after = ft_substr(line, dollar_place + len + 1, ft_strlen(line) - (dollar_place + len + 1));
	before = expend_join(before, env);
	after = expend_join(before, after);
	after = collector_dup(&all->collector, after);
	return (after);
}

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

void	check_tilde(t_all *all, t_card *node)
{
	t_card	*current;
	char	*home;

	home = NULL;
	current = node;
	while (current)
	{
		if (current->value[0] == '~' && current->value[1] == '\0')
		{
			home = ft_getenv(all->env, "HOME");
			current->value = collector_dup(&all->collector, home);
		}
		current = current->next;
	}
}

int	search_dollar(t_all *all, t_card *node, t_card *prev_node)
{
	int		i;
	char	open_quote;
	int		flag;

	flag = 0;
	i = 0;
	open_quote = '\0';
	while (node->value[0] != '\0' && node->value[i] != '\0')
	{
		open_quote = is_char_quote((node->value)[i], open_quote);
		if (open_quote != '\'' && prev_node->type != HEREDOC
			&& (node->value)[i] == '$' && node->value[i + 1] != '\0')
		{
			node->value = found_dollar((node->value), i, all);
			flag = 1;
		}
		if(node->value[i] == '\0')
			return (flag);
		i++;
	}
	return (flag);
}

void	check_for_expansion(t_all *all)
{
	int		flag;
	t_card	*node;
	t_card	*prev_node;

	flag = 0;
	node = all->card;
	prev_node = node;
	prev_node->type = node->type;
	while (node != NULL)
	{
		flag = search_dollar(all, node, prev_node);
		if (node->value[0] == '\0')
			check_node(node, prev_node);
		else if (flag == 1)
		{
			delim_node(all, node);
			flag = 0;
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
	len = strlen(str);
	quotes = quote_count(str);
	result = (char *)safe_malloc(&all->collector, len - quotes + 1);
	while (str[i])
	{
		if(skip_quotes(str, &open_quote, i))
		{
			i++;
			continue;
		}
		result[j] = str[i];
		j++;
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	quote_ingnore(t_all *all)
{
	t_card	*current;
	int		quotes;

	current = all->card;
	while (current)
	{
		quotes = 0;
		current->value = quote_ignore(all, current->value, quotes);
		if (current->type == HEREDOC && current->next && quotes != 0)
			current->next->here_flag = 1;
		current = current->next;
	}
}

void	expander(t_all *all)
{
	put_title(all);
	check_tilde(all, all->card);
	check_for_expansion(all);
	quote_ingnore(all);
}
