/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:40:17 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/19 09:50:30 by asezgin          ###   ########.fr       */
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
		// Only close if pipe was actually created
		if (pipefd[1] != -1)
			close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	// Note: When cmd->next is NULL, no pipe was created, so no cleanup needed
	// The pipefd array will contain -1 values from initialization
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

void	process_fork_cmd(t_cmd *cmd, t_all *all, int *prev_fd, int pipefd[2])
{
	pid_t	pid;

	signal_switch(2);
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
		exec_parent_process(cmd, all, prev_fd, pid);
		handle_pipe_parent(cmd, prev_fd, pipefd);
	}
	signal_switch(1);
}
