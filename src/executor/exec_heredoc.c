/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:50 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/24 19:25:46 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

static int	read_heredoc_input(int write_fd, char *eof, t_all *all)
{
	char	*line;

	line = readline("> ");
	while (line)
	{
		if (strcmp(line, eof) == 0)
		{
			free(line);
			break ;
		}
		if (check_here_flag(all->card, eof))
			line = here_expand(line, all);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	return (0);
}

char **last_heredoc(t_all *all)
{
	char **last;
	int heredoc_count;

	heredoc_count = 0;
	while (all->cmd->redirects)
	{
		if (all->cmd->redirects->type == HEREDOC)
		{
			last[heredoc_count] = all->cmd->redirects->filename;
			heredoc_count++;
		}
		all->cmd->redirects = all->cmd->redirects->next;
	}
	return (last);
}

void	heredoc_loop(t_all *all, char **last_heredocs, int heredoc_pipe[2], t_redirect *redir)
{
	int i;
	int n;

	i = ft_strlen(last_heredocs);
	n = 0;
	while (n < i && last_heredocs[n])
	{
		read_heredoc_input(heredoc_pipe[1], last_heredocs[n], all);
		n++;
	}
}
int	handle_heredoc_process(t_redirect *redir, t_all *all)
{
	int		heredoc_pipe[2];

	char **last_heredocs = last_heredoc(all);
	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe for heredoc");
		return (1);
	}
	heredoc_loop(all, last_heredocs, heredoc_pipe, redir);
	close(heredoc_pipe[1]);
	redir->fd = heredoc_pipe[0];
	if (dup2(redir->fd, STDIN_FILENO) == -1)
	{
		perror("dup2 for heredoc");
		close(redir->fd);
		return (1);
	}
	close(redir->fd);
	return (0);
}

int	check_here_flag(t_card *card, char *eof)
{
	t_card	*current;

	current = card;
	while (current)
	{
		if (current->here_flag == 1 && strcmp(current->value, eof) == 0)
			return (0);
		current = current->next;
	}
	return (1);
}
