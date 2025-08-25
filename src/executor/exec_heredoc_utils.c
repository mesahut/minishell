/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:03:31 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/25 15:04:46 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_heredoc_eof(char *eof)
{
	write(STDERR_FILENO, "minishell: warning: here-document ", 34);
	write(STDERR_FILENO, "delimited by end-of-file (wanted ", 33);
	write(STDERR_FILENO, eof, ft_strlen(eof));
	write(STDERR_FILENO, "')\n", 3);
	return (1);
}

int	process_heredoc_line(char *line, int write_fd, char *eof, t_all *all)
{
	if (strcmp(line, eof) == 0)
	{
		free(line);
		return (1);
	}
	if (check_here_flag(all->card, eof))
		line = here_expand(line, all);
	if (write_fd >= 0)
	{
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
	}
	free(line);
	return (0);
}

int	read_heredoc_input(int write_fd, char *eof, t_all *all)
{
	char		*line;
	extern int	g_signal;

	while (1)
	{
		if (g_signal == SIGINT)
			return (130);
		write(STDOUT_FILENO, "> ", 2);
		line = readline("");
		if (g_signal == SIGINT || !line)
		{
			if (line)
				free(line);
			if (g_signal == SIGINT)
				return (130);
			return (handle_heredoc_eof(eof));
		}
		if (process_heredoc_line(line, write_fd, eof, all))
			break ;
	}
	return (0);
}

int	heredoc_loop(t_all *all, char **heredocs, int pipefd[2])
{
	int	i;
	int	ret;

	i = 0;
	while (heredocs[i])
	{
		ret = read_heredoc_input(pipefd[1], heredocs[i], all);
		if (ret == 130)
			return (ret);
		i++;
	}
	return (0);
}
