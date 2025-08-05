/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:10:00 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/05 23:05:59 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void	exec_builtin_single(t_cmd *cmd, t_all *all, int prev_fd, int saved_stdin, int saved_stdout)
{
	// Setup input from previous command
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	
	// Save and setup redirections
	if (cmd->redirects)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_redirections(cmd, all);
	}
	
	// Execute builtin
	all->exit_status = exec_builtin(all, cmd);
	
	// Restore original file descriptors
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

void	exec_child_process(t_cmd *cmd, t_all *all, int prev_fd, int pipefd[2])
{
	// Setup input from previous command
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	
	// Setup redirections
	if (cmd->redirects)
		handle_redirections(cmd, all);

	// Setup output for next command
	if (cmd->next)
	{
		// Don't close pipefd[0] here, it will be used by the next command
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}

	// Execute command
	if (is_builtin(cmd->args[0]))
	{
		exit(exec_builtin(all, cmd));
	}
	else
	{
		char	*path;
		char	**envp;
		int		i;

		path = path_find(cmd->args[0]);
		if (path)
		{
			envp = list_to_envp(all->env);
			execve(path, cmd->args, envp);
			printf("%s: command not found\n", cmd->args[0]);
			
			// Free the path if it was dynamically allocated by path_find
			// path_find always allocates memory, so we should always free it
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
			
			reset_all(all);
			free_env(all->env);
			rl_clear_history();
			exit(1);
		}
		else
		{
			printf("%s: command not found\n", cmd->args[0]);
			exit(1);
		}
	}

	exit(1);
}

void	exec_parent_process(t_cmd *cmd, t_all *all, int *prev_fd, int pipefd[2], pid_t pid)
{
	// Close previous file descriptor
	if (*prev_fd != -1)
	{
		close(*prev_fd);
		*prev_fd = -1;
	}

	// Setup for next command or cleanup
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
		pipefd[1] = -1;
	}
	else
	{
		// For the last command, close the previous fd
		if (*prev_fd != -1)
		{
			close(*prev_fd);
			*prev_fd = -1;
		}
		// Close the write end of the current pipe if it's still open
		if (pipefd[1] != -1)
		{
			close(pipefd[1]);
			pipefd[1] = -1;
		}
		// Mark the read end as closed since it was passed to prev_fd and already closed
		pipefd[0] = -1;
	}

	waitpid(pid, &all->exit_status, 0);
}

void	exec_pipeline(t_all *all)
{
	t_cmd	*cmd;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		saved_stdin;
	int		saved_stdout;

	// Initialize variables
	cmd = all->cmd;
	pipefd[0] = -1;
	pipefd[1] = -1;
	prev_fd = -1;
	saved_stdin = -1;
	saved_stdout = -1;

	// Process each command
	while (cmd)
	{
		if (cmd->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		
		if (is_builtin(cmd->args[0]) && cmd->next == NULL)
		{
			// Execute single builtin without fork
			exec_builtin_single(cmd, all, prev_fd, saved_stdin, saved_stdout);
			break;
		}
		else
		{
			// Fork and execute command
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(1);
			}
			else if (pid == 0)
				exec_child_process(cmd, all, prev_fd, pipefd);
			else
				exec_parent_process(cmd, all, &prev_fd, pipefd, pid);
			cmd = cmd->next;
		}
	}
	
	// Final cleanup
	if (prev_fd != -1)
	{
		close(prev_fd);
		prev_fd = -1;
	}
	// Only cleanup pipe if it was actually created
	if (pipefd[0] != -1 || pipefd[1] != -1)
		cleanup_pipe(pipefd);
} 
