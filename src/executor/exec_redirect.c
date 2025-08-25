/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:44:28 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/25 08:47:56 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int	handle_redir_out(t_redirect *redir)
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

static int heredocs_handled = 0;

static void	handle_input_redirects(t_redirect *redir, t_all *all)
{
	if (redir->type == R_IN)
	{
		if (handle_redir_in(redir))
			exit(EXIT_FAILURE);
	}
	else if (redir->type == HEREDOC && !heredocs_handled)
	{
		signal_switch(3);
		if (handle_heredoc_process(redir, all))
			exit(EXIT_FAILURE);
		signal_switch(1);
		heredocs_handled = 1;
	}
}

static void	handle_error_redirects(t_redirect *redir)
{
	if (redir->type == R_ERR_OUT)
	{
		if (handle_redir_err_out(redir))
			exit(EXIT_FAILURE);
	}
	else if (redir->type == R_ERR_APPEND)
	{
		if (handle_redir_err_append(redir))
			exit(EXIT_FAILURE);
	}
}

static void	process_single_redirect(t_redirect *redir, t_all *all)
{
	if (redir->type == R_OUT || redir->type == R_APPEND)
		handle_output_redirects(redir);
	else if (redir->type == R_IN || redir->type == HEREDOC)
		handle_input_redirects(redir, all);
	else if (redir->type == R_ERR_OUT || redir->type == R_ERR_APPEND)
		handle_error_redirects(redir);
}

void	handle_redirections(t_cmd *cmd, t_all *all)
{
	t_redirect	*redir;

	heredocs_handled = 0; /* Her komut için reset et */
	redir = cmd->redirects;
	while (redir)
	{
		process_single_redirect(redir, all);
		redir = redir->next;
	}
}
