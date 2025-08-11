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

int redir_counter(t_card *start)
{
	t_card *current_card = start;
	int count = 0;

	while (current_card && current_card->type != PIPE)
	{
		if (current_card->type == R_APPEND || current_card->type == R_OUT ||
			current_card->type == HEREDOC || current_card->type == R_IN)
			count++;
		current_card = current_card->next;
	}
	return count;
}


int args_counter(t_card *start)
{
	t_card *current_card = start;
	int count = 0;

	while (current_card && current_card->type != PIPE)
	{
		if (current_card->type == WORD || current_card->type == -1)
			count++;
		else if (current_card->type == R_APPEND || current_card->type == R_OUT ||
				 current_card->type == HEREDOC || current_card->type == R_IN)
		{
			if (current_card->next)
				current_card = current_card->next;
		}
		current_card = current_card->next;
	}
	return count;
}


t_cmd *init_cmd(t_cmd *head_cmd, t_card *cursor, t_all *all)
{
	t_cmd *cmd;
	int arg_count;

	if (!all || !cursor)
		return NULL;

	arg_count = args_counter(cursor);
	cmd = (t_cmd *)safe_malloc(all, sizeof(t_cmd));
	if (!cmd)
		return NULL;

	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = head_cmd;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->args_count = arg_count;

	cmd->args = (char **)safe_malloc(all, sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return NULL;

	for (int i = 0; i <= arg_count; i++)
		cmd->args[i] = NULL;

	cmd->redirect_count = redir_counter(cursor);
	if (cmd->redirect_count > 0)
	{
		cmd->redirects = NULL; // Her biri tek tek malloc ile eklenecek
	}

	return cmd;
}


void    put_node(t_cmd **head_cmd, t_cmd *new_cmd)
{
	t_cmd *temp;
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

void set_cmd(t_card *cursor, t_all *all, t_cmd *current_cmd)
{
	t_card *current_card;
	int i;

	if (!cursor || !all || !current_cmd)
		return;
	current_card = cursor;
	i = 0;

	while (current_card && current_card->type != PIPE)
	{
		if (current_card->type == R_APPEND || current_card->type == R_OUT ||
			current_card->type == HEREDOC || current_card->type == R_IN)
		{
			t_redirect *redir = safe_malloc(all, sizeof(t_redirect));
			redir->type = current_card->type;
			if (current_card->next && current_card->next->value)
				redir->filename = current_card->next->value;
			else
				redir->filename = NULL;
			redir->value = NULL;
			redir->fd = -1;
			redir->next = NULL;
			if (!current_cmd->redirects)		
				current_cmd->redirects = redir;
			else
			{
				t_redirect *tmp = current_cmd->redirects;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = redir;
			}
		}
		else if ((current_card->type == WORD || current_card->type == -1)
				&& current_card->value)
		{
			if (i < current_cmd->args_count)
			{
				current_cmd->args[i] = current_card->value;
				i++;
			}
		}
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
