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
		if (is_redir(current_card->type))
		{
			set_redir(all, current_cmd, current_card, current_card->type);
			current_card = current_card->next;
			if (current_card)
				current_card = current_card->next;
		}
		else if ((current_card->type == WORD || current_card->type == -1)
			&& current_card->value)
		{
			if (current_card->value[0] == '2'
				&& current_card->value[1] == '\0'
				&& current_card->next && current_card->next->type == R_OUT)
			{
				set_redir(all, current_cmd, current_card, R_ERR_OUT);
				current_card = current_card->next;
				if (current_card)
					current_card = current_card->next;
			}
			else if (current_card->value[0] == '2'
				&& current_card->value[1] == '\0'
				&& current_card->next && current_card->next->type == R_APPEND)
			{
				set_redir(all, current_cmd, current_card, R_ERR_APPEND);
				current_card = current_card->next;
				if (current_card)
					current_card = current_card->next;
			}
			else if (i < current_cmd->args_count)
			{
				current_cmd->args[i] = current_card->value;
				i++;
				current_card = current_card->next;
			}
			else
				current_card = current_card->next;
		}
		else
			current_card = current_card->next;
	}
	current_cmd->args[i] = NULL;
}

static int	preprocess_heredocs(t_all *all)
{
	t_cmd		*cmd;
	t_redirect	*redir;
	int			ret;

	cmd = all->cmd;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				signal_switch(3);
				ret = handle_all_heredocs_for_cmd(cmd, all);
				signal_switch(1);
				if (ret != 0)
					return (ret);
				break;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	parser(t_all *all)
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
	if (preprocess_heredocs(all) != 0)
	{
		all->exit_status = 130;
		all->exit_flag = 1;
		return(all->exit_flag);
	}
	return (0);
}
