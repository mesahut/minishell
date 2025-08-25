/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:39:25 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/25 17:20:51 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

static void	init_pipeline_vars(t_all *all, t_cmd **cmd, int *prev_fd)
{
	*cmd = all->cmd;
	*prev_fd = -1;
}

static int	process_single_cmd(t_cmd *cmd, t_all *all, int *prev_fd)
{
	int		pipefd[2];

	if (process_builtin_cmd(cmd, all, *prev_fd))
		return (1);
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	process_fork_cmd(cmd, all, prev_fd, pipefd);
	return (0);
}

void	exec_signal_wait(t_all *all)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			all->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			all->exit_status = 128 + WTERMSIG(status);
	}
}

void	exec(t_all *all)
{
	t_cmd	*cmd;
	int		prev_fd;
	int		is_pipeline;

	init_pipeline_vars(all, &cmd, &prev_fd);
	is_pipeline = (cmd && cmd->next != NULL);
	while (cmd)
	{
		if (process_single_cmd(cmd, all, &prev_fd))
			break ;
		cmd = cmd->next;
	}
	if (prev_fd != -1)
	{
		close(prev_fd);
	}
	if (is_pipeline)
	{
		signal_switch(2);
		exec_signal_wait(all);
		signal_switch(1);
	}
}
