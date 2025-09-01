/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_clean.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 09:54:46 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/09/01 16:31:16 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

void	cleanup_heredoc_fds(t_cmd *cmd)
{
	t_redirect	*redir;

	if (!cmd)
		return ;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == HEREDOC && redir->fd > 0)
		{
			close(redir->fd);
			redir->fd = -1;
		}
		redir = redir->next;
	}
}

void	cleanup_all_heredoc_fds(t_all *all)
{
	t_cmd	*cmd;

	if (!all || !all->cmd)
		return ;
	cmd = all->cmd;
	while (cmd)
	{
		cleanup_heredoc_fds(cmd);
		cmd = cmd->next;
	}
}
