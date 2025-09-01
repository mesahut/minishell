/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:16:34 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/09/01 10:35:42 by mayilmaz         ###   ########.fr       */
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

int	is_redir(int type)
{
	if (type == R_APPEND || type == R_OUT || type == HEREDOC
		|| type == R_IN || type == R_ERR_OUT || type == R_ERR_APPEND)
	{
		return (1);
	}
	return (0);
}

int	preprocess_heredocs(t_all *all)
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
				break ;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
