/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:10:00 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 13:35:48 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int	handle_redir_err_out(t_redirect *redir)
{
	redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(redir->fd, STDERR_FILENO) == -1)
	{
		perror("dup2 for stderr");
		close(redir->fd);
		return (1);
	}
	close(redir->fd);
	return (0);
}

int	handle_redir_err_append(t_redirect *redir)
{
	redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	if (dup2(redir->fd, STDERR_FILENO) == -1)
	{
		perror("dup2 for stderr append");
		close(redir->fd);
		return (1);
	}
	close(redir->fd);
	return (0);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static void	exec_with_redirect(t_cmd *cmd, t_all *all)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	handle_redirections(cmd, all);
	if (exec_builtin(all, cmd, 1))
		all->exit_status = EXIT_FAILURE;
	restore_fds(saved_stdin, saved_stdout);
}

void	exec_builtin_single(t_cmd *cmd, t_all *all, int prev_fd)
{
	int	saved_stdin;

	saved_stdin = -1;
	if (prev_fd != -1)
	{
		saved_stdin = dup(STDIN_FILENO);
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->redirects)
		exec_with_redirect(cmd, all);
	else
	{
		if (exec_builtin(all, cmd, 1))
		{
			all->exit_status = EXIT_FAILURE;
			all->exit_flag = 1;
		}
	}
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
}
