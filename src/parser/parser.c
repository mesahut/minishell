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

void	put_node(t_cmd **head_cmd, t_cmd *new_cmd)
{
	t_cmd	*temp;

	temp = NULL;
	if (*head_cmd == NULL)
	{
		*head_cmd = new_cmd;
	}
	else
	{
		temp = *head_cmd;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_cmd;
		new_cmd->prev = temp;
	}
}

void	put_redir(t_redirect *redir, t_cmd *current_cmd, t_redirect *tmp)
{
	if (!current_cmd->redirects)
		current_cmd->redirects = redir;
	else
	{
		tmp = current_cmd->redirects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

void	check_init_cmd(t_all *all, t_cmd *cmd, t_card *card, int *palce)
{
	int	i;

	i = *palce;
	if (is_redir(card->type))
		set_redir(all, cmd, card, card->type);
	else if ((card->type == WORD || card->type == -1)
		&& card->value)
	{
		if (card->value[0] == '2'
			&& card->value[1] == '\0'
			&& card->next && card->next->type == R_OUT)
			set_redir(all, cmd, card, R_ERR_OUT);
		else if (card->value[0] == '2'
			&& card->value[1] == '\0'
			&& card->next && card->next->type == R_APPEND)
			set_redir(all, cmd, card, R_ERR_APPEND);
		else if (i < cmd->args_count)
		{
			cmd->args[i] = card->value;
			i++;
		}
	}
	*palce = i;
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
		check_init_cmd(all, current_cmd, current_card, &i);
		current_card = current_card->next;
	}
	current_cmd->args[i] = NULL;
}

void	parser(t_all *all)
{
	t_cmd	*head_cmd;
	t_cmd	*current_cmd;
	t_card	*cursor;
	t_card	*old_card;

	cursor = all->card;
	head_cmd = NULL;
	while (cursor != NULL)
	{
		current_cmd = init_cmd(head_cmd, cursor, all);
		if (!current_cmd)
			return ;
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
}
