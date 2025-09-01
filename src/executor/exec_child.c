/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:42:31 by asezgin           #+#    #+#             */
/*   Updated: 2025/09/01 16:28:54 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdio.h>

void	set_out(t_all *all, int pipefd[2])
{
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 for stdout");
		close(pipefd[1]);
		if (pipefd[0] != -1)
			close(pipefd[0]);
		reset_all(all, 1);
	}
	close(pipefd[1]);
}

static void	setup_child_io(t_cmd *cmd, int prev_fd, int pipefd[2], t_all *all)
{
	if ((prev_fd != -1 && prev_fd != STDIN_FILENO))
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 for stdin");
			close(prev_fd);
			reset_all(all, 1);
		}
		close(prev_fd);
	}
	if (cmd->next && pipefd[1] != -1)
		set_out(all, pipefd);
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (prev_fd != -1 && prev_fd != STDIN_FILENO)
		close(prev_fd);
}

static void	execute_with_path(char *path, t_cmd *cmd, t_all *all)
{
	char	**envp;
	int		i;

	envp = list_to_envp(all);
	execve(path, cmd->args, envp);
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
	if (path[0] == '.' && path[1] == '/')
	{
		printf("minishell: %s: is a directory\n", cmd->args[0]);
		reset_all(all, 126);
	}
	reset_all(all, EXIT_FAILURE);
}

static void	execute_child_cmd(t_cmd *cmd, t_all *all)
{
	char	*path;

	path = path_find(cmd->args[0], all);
	if (path)
		execute_with_path(path, cmd, all);
	else if (cmd->args[0][0] == '\0')
	{
		printf("'': command not found\n");
		reset_all(all, 127);
	}
	if ((cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '/' && cmd->args[0][2] != '\0')
		|| (cmd->args[0][0] == '/' && cmd->args[0][1] != '\0'))
		directory_checker(cmd, all);
	else
		path_error(cmd, all);
}

void	exec_child_process(t_cmd *cmd, t_all *all, int prev_fd, int pipefd[2])
{
	int	n;

	n = 0;
	setup_child_io(cmd, prev_fd, pipefd, all);
	if (cmd->redirects)
		handle_redirections(cmd);
	if (is_builtin(cmd->args[0]) || ft_strcmp(cmd->args[0], "exit") == 0)
	{
		n = exec_builtin(all, cmd, 0);
		reset_all(all, n);
	}
	else
		execute_child_cmd(cmd, all);
	exit(EXIT_FAILURE);
}
