/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:50 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/19 15:15:06 by asezgin          ###   ########.fr       */
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

int	handle_heredoc_process(t_redirect *redir, t_all *all)
{
	int		heredoc_pipe[2];

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe for heredoc");
		return (1);
	}
	read_heredoc_input(heredoc_pipe[1], redir->filename, all);
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
