/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:00:00 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/19 09:19:24 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int	handle_heredoc_process(t_redirect *redir, t_all *all);

static int	handle_redir_out(t_redirect *redir)
{
	redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(redir->fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 for output");
		close(redir->fd);
		return (1);
	}
	close(redir->fd);
	return (0);
}

static int	handle_redir_append(t_redirect *redir)
{
	redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(redir->fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 for append");
		close(redir->fd);
		return (1);
	}
	close(redir->fd);
	return (0);
}

static int	handle_redir_in(t_redirect *redir)
{
	redir->fd = open(redir->filename, O_RDONLY);
	if (redir->fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(redir->fd, STDIN_FILENO) == -1)
	{
		perror("dup2 for input");
		close(redir->fd);
		return (1);
	}
	close(redir->fd);
	return (0);
}

static void	process_single_redirect(t_redirect *redir, t_all *all)
{
	if (redir->type == R_OUT)
	{
		if (handle_redir_out(redir))
			exit(EXIT_FAILURE);
	}
	else if (redir->type == R_APPEND)
	{
		if (handle_redir_append(redir))
			exit(EXIT_FAILURE);
	}
	else if (redir->type == R_IN)
	{
		if (handle_redir_in(redir))
			exit(EXIT_FAILURE);
	}
	else if (redir->type == HEREDOC)
	{
		signal_switch(3);
		if (handle_heredoc_process(redir, all))
			exit(EXIT_FAILURE);
		signal_switch(1);
	}
}

void	handle_redirections(t_cmd *cmd, t_all *all)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		process_single_redirect(redir, all);
		redir = redir->next;
	}
}
