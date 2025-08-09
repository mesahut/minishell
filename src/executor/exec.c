/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:50:51 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/09 19:27:59 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <unistd.h>     // fork, execve
#include <sys/wait.h>   // waitpid, WIFEXITED
#include <stdlib.h>     // exit
#include <stdio.h>      // perror, fprintf
#include <fcntl.h>

void	handle_redirections(t_cmd *cmd, t_all *all)
{
	t_redirect *redir = cmd->redirects;

	while (redir)
	{
		if (redir->type == R_OUT)
		{
			redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (redir->fd < 0)
			{
				perror(redir->filename);
				exit(1);
			}
			if (dup2(redir->fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 for output");
				close(redir->fd);
				exit(1);
			}
			close(redir->fd);
		}
		else if (redir->type == R_APPEND)
		{
			redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (redir->fd < 0)
			{
				perror(redir->filename);
				exit(1);
			}
			if (dup2(redir->fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 for append");
				close(redir->fd);
				exit(1);
			}
			close(redir->fd);
		}
		else if (redir->type == R_IN)
		{
			redir->fd = open(redir->filename, O_RDONLY);
			if (redir->fd < 0)
			{
				perror(redir->filename);
				exit(1);
			}
			if (dup2(redir->fd, STDIN_FILENO) == -1)
			{
				perror("dup2 for input");
				close(redir->fd);
				exit(1);
			}
			close(redir->fd);
		}
		else if (redir->type == HEREDOC)
		{
			int heredoc_pipe[2];
			if (pipe(heredoc_pipe) == -1)
			{
				perror("pipe for heredoc");
				exit(1);
			}

			char *line;
			while ((line = readline("> ")) != NULL)
			{
				if (strcmp(line, redir->filename) == 0)
				{
					free(line);
					break;
				}
				if (check_here_flag(all->card, redir->filename))
					line = here_expand(line, all);
				write(heredoc_pipe[1], line, ft_strlen(line));
				write(heredoc_pipe[1], "\n", 1);
				free(line);
			}
			close(heredoc_pipe[1]);
			redir->fd = heredoc_pipe[0];

			if (dup2(redir->fd, STDIN_FILENO) == -1)
			{
				perror("dup2 for heredoc");
				close(redir->fd);
				exit(1);
			}
			close(redir->fd);
		}
		redir = redir->next;
	}
}



void	exec(t_all *all)
{
	exec_pipeline(all);
}


