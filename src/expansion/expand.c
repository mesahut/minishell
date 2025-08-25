/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:15:07 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/23 18:15:07 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*found_dollar(char *line, int dollar_place, t_all *all)
{
	char	*before;
	char	*after;
	char	*env;
	int		len;

	len = 0;
	before = ft_substr(line, 0, dollar_place, all);
	if (line[dollar_place + 1] == '?')
		return (handle_exit_status(all, dollar_place, before, line));
	while (line[dollar_place + len + 1] != '\0'
		&& (ft_isalnum(line[dollar_place + len + 1]) == 1))
		len++;
	after = ft_substr(line, dollar_place + 1, len, all);
	env = ft_getenv(all->env, after);
	env = ft_strdup(env, all);
	free(after);
	after = ft_substr(line, dollar_place + len + 1,
			ft_strlen(line) - (dollar_place + len + 1), all);
	before = expend_join(before, env, all);
	after = expend_join(before, after, all);
	after = collector_dup(all, after);
	return (after);
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
			if (node->value == 0)
				return (-1);
			flag = 1;
		}
		if (node->value[i] == '\0')
			return (flag);
		i++;
	}
	return (flag);
}

int	check_for_expansion(t_all *all)
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
		if (flag == -1)
			return (1);
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
	return (0);
}

int	expander(t_all *all)
{
	put_title(all);
	check_tilde(all, all->card);
	if (check_for_expansion(all) == 1)
		return (1);
	if (syntax_checker(all) == 1)
		return (1);
	del_quote(all);
	return (0);
}
