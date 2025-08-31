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
	write(STDERR_FILENO, "delimited by end-of-file (wanted `", 35);
	write(STDERR_FILENO, eof, ft_strlen(eof));
	write(STDERR_FILENO, "')\n", 3);
	return (0);
}

static char	*expand_heredoc_line(char *line, t_all *all, int quoted)
{
	char	*expanded;

	if (quoted)
	{
		// If delimiter was quoted, no variable expansion
		expanded = ft_strdup(line, all);
	}
	else
	{
		// If delimiter was not quoted, do variable expansion
		// here_expand frees its input, so we need to make a copy first
		char *line_copy = ft_strdup(line, all);
		expanded = here_expand(line_copy, all);
	}
	return (expanded);
}

static int	write_line_to_pipe(char *line, int write_fd)
{
	if (write_fd >= 0)
	{
		if (write(write_fd, line, ft_strlen(line)) == -1)
			return (-1);
		if (write(write_fd, "\n", 1) == -1)
			return (-1);
	}
	return (0);
}

int	process_heredoc_line(char *line, int write_fd, char *eof, t_all *all, int quoted)
{
	char	*expanded_line;
	int		ret;

	if (ft_strcmp(line, eof) == 0)
	{
		free(line);
		return (1); // End of heredoc
	}

	expanded_line = expand_heredoc_line(line, all, quoted);
	free(line);
	
	if (!expanded_line)
		return (-1); // Error
	
	ret = write_line_to_pipe(expanded_line, write_fd);
	free(expanded_line);
	
	if (ret == -1)
		return (-1); // Write error
	
	return (0); // Continue reading
}

int	read_heredoc_input(int write_fd, char *delimiter, t_all *all, int quoted)
{
	char	*line;
	int		ret;

	while (1)
	{
		line = readline("> ");
		
		// Handle EOF (Ctrl+D)
		if (line == NULL)
		{
			handle_heredoc_eof(delimiter);
			break;
		}

		// Handle SIGINT (Ctrl+C)
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			free(line);
			return (130);
		}

		// Process the line
		ret = process_heredoc_line(line, write_fd, delimiter, all, quoted);
		
		if (ret == 1)
			break; // End of heredoc reached
		else if (ret == -1)
		{
			// Error occurred
			return (1);
		}
		// ret == 0: continue reading
	}
	
	return (0);
}
