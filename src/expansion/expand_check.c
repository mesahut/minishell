/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 21:44:21 by asezgin           #+#    #+#             */
/*   Updated: 2025/09/01 13:48:42 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

char	*handle_exit_status(t_all *all, int dollar, char *before, char *line)
{
	char	*status_str;
	char	*after;

	status_str = ft_itoa(all->exit_status, all);
	after = ft_substr(line, dollar + 2,
			ft_strlen(line) - (dollar + 2), all);
	before = expend_join(before, status_str, all);
	after = expend_join(before, after, all);
	after = collector_dup(all, after);
	return (after);
}

int	syntax_error(t_all *all)
{
	all->exit_status = 2;
	printf("syntax error\n");
	return (1);
}

int	syntax_checker(t_all *all)
{
	t_card	*current;

	current = all->card;
	if (current->type == PIPE)
		return (syntax_error(all));
	while (current)
	{
		if (current->type == PIPE && (!current->next
				|| current->next->type == PIPE))
			return (syntax_error(all));
		if ((current->type == R_OUT || current->type == R_IN
				|| current->type == R_APPEND || current->type == HEREDOC)
			&& (!current->next || current->next->type != WORD))
			return (syntax_error(all));
		current = current->next;
	}
	return (0);
}
