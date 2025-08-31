/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:49:39 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/31 20:41:31 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	redir_counter(t_card *start)
{
	t_card	*current_card;
	int		count;

	count = 0;
	current_card = start;
	while (current_card && current_card->type != PIPE)
	{
		if (is_redir(current_card->type))
			count++;
		current_card = current_card->next;
	}
	return (count);
}

int	args_counter(t_card *start)
{
	t_card	*current_card;
	int		count;

	count = 0;
	current_card = start;
	while (current_card && current_card->type != PIPE)
	{
		if (current_card->type == WORD || current_card->type == -1)
		{
			count++;
			current_card = current_card->next;
		}
		else if (is_redir(current_card->type))
		{
			current_card = current_card->next;
			if (current_card)
				current_card = current_card->next;
		}
		else
			current_card = current_card->next;
	}
	return (count);
}

void	init_cmd_value(t_all *all, t_cmd *cmd, t_cmd *head_cmd, int arg_count)
{
	int	i;

	i = 0;
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = head_cmd;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->args_count = arg_count;
	cmd->args = (char **)safe_malloc(all, sizeof(char *) * (arg_count + 1));
	while (i <= arg_count)
	{
		cmd->args[i] = NULL;
		i++;
	}
}

t_cmd	*init_cmd(t_cmd *head_cmd, t_card *cursor, t_all *all)
{
	t_cmd	*cmd;
	int		arg_count;

	if (!all || !cursor)
		return (NULL);
	arg_count = args_counter(cursor);
	cmd = (t_cmd *)safe_malloc(all, sizeof(t_cmd));
	init_cmd_value(all, cmd, head_cmd, arg_count);
	cmd->redirect_count = redir_counter(cursor);
	if (cmd->redirect_count > 0)
		cmd->redirects = NULL;
	return (cmd);
}

void	set_redir(t_all *all, t_cmd *current_cmd, t_card *card, int type)
{
	t_redirect	*tmp;
	t_redirect	*redir;

	tmp = NULL;
	redir = safe_malloc(all, sizeof(t_redirect));
	redir->type = type;
	if (card->next && card->next->value)
	{
		redir->filename = card->next->value;
		card = card->next;
	}
	else
		redir->filename = NULL;
	redir->value = NULL;
	redir->fd = -1;
	redir->next = NULL;
	put_redir(redir, current_cmd, tmp);
}
