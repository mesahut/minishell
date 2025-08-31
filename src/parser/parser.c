/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:14:08 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/10 16:14:08 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_help( t_all *all, t_cmd *current_cmd, t_card **current_card, int *i)
{
	if (redir_case(current_card, R_OUT) == 1)
	{
		set_redir(all, current_cmd, (*current_card), R_ERR_OUT);
		(*current_card) = (*current_card)->next;
		if ((*current_card))
			(*current_card) = (*current_card)->next;
	}
	else if (redir_case(current_card, R_APPEND))
	{
		set_redir(all, current_cmd, (*current_card), R_ERR_APPEND);
		(*current_card) = (*current_card)->next;
		if ((*current_card))
			(*current_card) = (*current_card)->next;
	}
	else if ((*i) < current_cmd->args_count)
	{
		current_cmd->args[(*i)] = (*current_card)->value;
		(*i)++;
		(*current_card) = (*current_card)->next;
	}
	else
		(*current_card) = (*current_card)->next;
}

void	redir_put(t_all *all, t_cmd *current_cmd, t_card **current_card, int *i)
{
	if (is_redir((*current_card)->type))
	{
		set_redir(all, current_cmd, (*current_card), (*current_card)->type);
		(*current_card) = (*current_card)->next;
		if ((*current_card))
			(*current_card) = (*current_card)->next;
	}
	else if (((*current_card)->type == WORD || (*current_card)->type == -1)
		&& (*current_card)->value)
	{
		ft_help(all, current_cmd, current_card, i);
	}
	else
		(*current_card) = (*current_card)->next;
}

void	set_cmd(t_card *cursor, t_all *all, t_cmd *current_cmd)
{
	t_card		*current_card;
	int			i;

	if (!cursor || !all || !current_cmd)
		return ;
	current_card = cursor;
	i = 0;
	while (current_card && current_card->type != PIPE)
	{
		redir_put(all, current_cmd, &current_card, &i);
	}
	current_cmd->args[i] = NULL;
}

int	parser_cycle(t_all *all, t_card *cursor)
{
	t_cmd	*head_cmd;
	t_cmd	*current_cmd;
	t_card	*old_card;

	current_cmd = NULL;
	old_card = NULL;
	head_cmd = NULL;
	while (cursor != NULL)
	{
		current_cmd = init_cmd(head_cmd, cursor, all);
		if (!current_cmd)
			return (0);
		put_node(&head_cmd, current_cmd);
		old_card = all->card;
		all->card = cursor;
		set_cmd(cursor, all, current_cmd);
		all->card = old_card;
		while (cursor && cursor->type != PIPE)
			cursor = cursor->next;
		if (cursor)
			cursor = cursor->next;
	}
	all->cmd = head_cmd;
	return (1);
}

int	parser(t_all *all)
{
	t_card	*cursor;

	cursor = all->card;
	if (parser_cycle(all, cursor) == 0)
		return (0);
	if (preprocess_heredocs(all) != 0)
	{
		cleanup_all_heredoc_fds(all);
		all->exit_status = 130;
		all->exit_flag = 1;
		return (all->exit_flag);
	}
	return (0);
}
