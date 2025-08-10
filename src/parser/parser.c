/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:24:41 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/10 10:24:41 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	redir_counter(t_card *start)
{
	t_card	*current_card;
	int		count;

	current_card = start;
	count = 0;
	while (current_card && current_card->type != PIPE)
	{
		if (current_card->type == R_APPEND
			|| current_card->type == R_OUT
			|| current_card->type == HEREDOC
			|| current_card->type == R_IN)
			count++;
		current_card = current_card->next;
	}
	return (count);
}

int	args_counter(t_card *start)
{
	t_card	*current_card;
	int		count;

	current_card = start;
	count = 0;
	while (current_card && current_card->type != PIPE)
	{
		if (current_card->type == WORD || current_card->type == -1)
			count++;
		else if (current_card->type == R_APPEND
			|| current_card->type == R_OUT
			|| current_card->type == HEREDOC
			|| current_card->type == R_IN)
		{
			if (current_card->next)
				current_card = current_card->next;
		}
		current_card = current_card->next;
	}
	return (count);
}

t_cmd	*create_cmd_base(t_cmd *prev, t_all *all)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)safe_malloc(&all->collector, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = prev;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->args_count = 0;
	cmd->redirect_count = 0;
	return (cmd);
}

int	setup_cmd_args(t_cmd *cmd, int arg_count, t_all *all)
{
	int	i;

	i = 0;
	cmd->args_count = arg_count;
	cmd->args = (char **)safe_malloc(&all->collector,
			sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);
	while (i <= arg_count)
	{
		cmd->args[i] = NULL;
		i++;
	}
	return (1);
}

t_cmd	*init_cmd(t_cmd *head_cmd, t_card *cursor, t_all *all)
{
	int		arg_count;
	t_cmd	*cmd;

	if (!all || !cursor)
		return (NULL);
	arg_count = args_counter(cursor);
	cmd = create_cmd_base(head_cmd, all);
	if (!cmd)
		return (NULL);
	if (!setup_cmd_args(cmd, arg_count, all))
		return (NULL);
	cmd->redirect_count = redir_counter(cursor);
	cmd->redirects = (t_redirect *)safe_malloc(&all->collector, (sizeof(t_redirect) * cmd->redirect_count));
	return (cmd);
}

void	put_node(t_cmd **head_cmd, t_cmd *new_cmd)
{
	t_cmd	*temp;

	if (*head_cmd == NULL)
		*head_cmd = new_cmd;
	else
	{
		temp = *head_cmd;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_cmd;
		new_cmd->prev = temp;
	}
}

int	is_redir(int type)
{
	return (type == R_APPEND || type == R_OUT
		|| type == HEREDOC || type == R_IN);
}

int	is_arg(t_card *card)
{
	return ((card->type == WORD || card->type == -1) && card->value);
}

int	handle_arg(t_card *card, t_cmd *cmd, int i)
{
	if (i < cmd->args_count)
	{
		cmd->args[i] = card->value;
		return (1);
	}
	return (0);
}

void	add_redir_to_list(t_redirect **head, t_redirect *new_redir)
{
	if (!*head)
		*head = new_redir;
	else
	{
		t_redirect	*tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
}

int	handle_redir(t_card **card_ptr, t_cmd *cmd, t_all *all)
{
	t_card		*card = *card_ptr;
	t_redirect	*redir;

	redir = NULL;
	//if (card->next && card->next->value)
	//{
	redir->type = card->type;
	//redir->filename = card->next->value;
	redir->value = ">";
	redir->fd = -1;
	redir->next = NULL;
	add_redir_to_list(&cmd->redirects, redir);
	*card_ptr = card->next;
	//}
	//else
	//{<
	//	printf("%s: syntax error\n", cmd->args[0]);
	//}
	return (0);
}

void	set_cmd(t_card *cursor, t_all *all, t_cmd *current_cmd)
{
	t_card	*current_card = cursor;
	int		i = 0;

	if (!cursor || !all || !current_cmd)
		return ;
	while (current_card && current_card->type != PIPE)
	{
		if (is_redir(current_card->type))
		{
			i += handle_redir(&current_card, current_cmd, all);
		}
		else if (is_arg(current_card))
			i += handle_arg(current_card, current_cmd, i);
		current_card = current_card->next;
	}
	current_cmd->args[i] = NULL;
}

void	parser(t_all *all)
{
	t_cmd	*head_cmd = NULL;
	t_cmd	*current_cmd;
	t_card	*cursor = all->card;

	while (cursor != NULL)
	{
		current_cmd = init_cmd(head_cmd, cursor, all);
		if (!current_cmd)
			return ;
		put_node(&head_cmd, current_cmd);
		set_cmd(cursor, all, current_cmd);
		while (cursor && cursor->type != PIPE)
			cursor = cursor->next;
		if (cursor)
			cursor = cursor->next;
	}
	all->cmd = head_cmd;
}
