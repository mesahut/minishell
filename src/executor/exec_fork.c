/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:40:17 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/25 14:14:08 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

static void	handle_pipe_parent(t_cmd *cmd, int *prev_fd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipefd[1] != -1)
			close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

int	process_builtin_cmd(t_cmd *cmd, t_all *all, int prev_fd)
{
	if (is_builtin(cmd->args[0]) && cmd->next == NULL)
	{
		exec_builtin_single(cmd, all, prev_fd);
		return (1);
	}
	return (0);
}

void	exec_parent_process(t_cmd *cmd, t_all *all, int *prev_fd, pid_t pid)
{
	int	status;

	if (*prev_fd != -1)
	{
		close(*prev_fd);
		*prev_fd = -1;
	}
	if (cmd->next == NULL)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			all->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			all->exit_status = 128 + WTERMSIG(status);
	}
}

void	process_fork_cmd(t_cmd *cmd, t_all *all, int *prev_fd, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		exec_child_process(cmd, all, *prev_fd, pipefd);
	else
	{
		signal_switch(2);
		exec_parent_process(cmd, all, prev_fd, pid);
		handle_pipe_parent(cmd, prev_fd, pipefd);
	}
	signal_switch(1);
}
