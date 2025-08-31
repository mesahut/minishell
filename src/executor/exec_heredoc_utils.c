/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:03:31 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 21:17:50 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

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
	if (ft_strcmp(line, eof) == 0)
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

int	read_heredoc_input(int write_fd, char **heredoc, t_all *all)
{
	char	*line;
	int		i;
	int		count;
	int		is_last;

	count = 0;
	i = 0;
	while (heredoc[count])
		count++;
	while (i < count)
	{
		is_last = (i == count - 1);
		while (1)
		{
			line = readline("> ");
			if (line == NULL)
			{
				handle_heredoc_eof(heredoc[i]);
				break ;
			}
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				free(line);
				return (130);
			}
			if (strcmp(line, heredoc[i]) == 0)
			{
				free(line);
				break ;
			}
			if (is_last)
				process_heredoc_line(line, write_fd, heredoc[i], all);
			else
				free (line);
		}
		i++;
	}
	return (0);
}
