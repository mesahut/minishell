/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:42:31 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/27 14:28:09 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void	handle_output_redirects(t_redirect *redir)
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
}

static void	setup_child_io(t_cmd *cmd, int prev_fd, int pipefd[2])
{
	if (prev_fd != -1 && prev_fd != STDIN_FILENO)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 for stdin");
			close(prev_fd);
			exit(EXIT_FAILURE);
		}
		close(prev_fd);
	}
	if (cmd->next && pipefd[1] != -1)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 for stdout");
			close(pipefd[1]);
			if (pipefd[0] != -1)
				close(pipefd[0]);
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
	}
	if (cmd->next && pipefd[0] != -1)
		close(pipefd[0]);
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
	exit(EXIT_FAILURE);
}

static void	execute_child_cmd(t_cmd *cmd, t_all *all)
{
	char	*path;

	path = path_find(cmd->args[0], all);
	if (path)
		execute_with_path(path, cmd, all);
	else
	{
		if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
			fprintf(stderr, "%s: No such file or directory\n", cmd->args[0]);
		else
			fprintf(stderr, "%s: command not found\n", cmd->args[0]);
		reset_all(all, EXIT_COMMAND_NOT_FOUND);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
}

void	exec_child_process(t_cmd *cmd, t_all *all, int prev_fd, int pipefd[2])
{
	int	n;

	n = 0;
	signal_switch(1);
	setup_child_io(cmd, prev_fd, pipefd);
	if (cmd->redirects)
		handle_redirections(cmd, all);
	if (strcmp(cmd->args[0], "exit") == 0)
	{
		free_env(all->env);
		clean_malloc(all);
		exit(n);
	}
	if (is_builtin(cmd->args[0]))
	{
		n = exec_builtin(all, cmd);
	}
	else
		execute_child_cmd(cmd, all);
	exit(EXIT_FAILURE);
}
