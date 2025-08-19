/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:42:31 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/19 09:50:30 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

static void	setup_child_io(t_cmd *cmd, int prev_fd, int pipefd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next && pipefd[1] != -1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		// Close the read end of the pipe in child
		if (pipefd[0] != -1)
			close(pipefd[0]);
	}
}

static void	cleanup_and_exit(t_all *all, int status)
{
	reset_all(all, 0);
	rl_clear_history();
	exit(status);
}

static void	execute_with_path(char *path, t_cmd *cmd, t_all *all)
{
	char	**envp;
	int		i;

	envp = list_to_envp(all);
	execve(path, cmd->args, envp);
	free(path);
	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
			cleanup_and_exit(all, EXIT_FAILURE);
}

static void	execute_child_cmd(t_cmd *cmd, t_all *all)
{
	char	*path;

	path = path_find(cmd->args[0], all);
	if (path)
		execute_with_path(path, cmd, all);
	else
	{
		all->exit_status = EXIT_COMMAND_NOT_FOUND;
		if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
			printf("%s: No such file or directory\n", cmd->args[0]);
		else
			printf("%s: command not found\n", cmd->args[0]);
		cleanup_and_exit(all, EXIT_COMMAND_NOT_FOUND);
	}
}

void	exec_child_process(t_cmd *cmd, t_all *all, int prev_fd, int pipefd[2])
{
	setup_child_io(cmd, prev_fd, pipefd);
	if (cmd->redirects)
		handle_redirections(cmd, all);
	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(all, cmd));
	else
		execute_child_cmd(cmd, all);
	exit(EXIT_FAILURE);
}
