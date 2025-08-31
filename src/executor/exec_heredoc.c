/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:50 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 21:39:55 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../../include/minishell.h"

static int	handle_heredoc_error(int heredoc_pipe[2], int ret)
{
	if (heredoc_pipe[0] != -1)
		close(heredoc_pipe[0]);
	if (heredoc_pipe[1] != -1)
		close(heredoc_pipe[1]);
	if (ret == 130)
		freopen("/dev/tty", "r", stdin);
	return (ret);
}

static char	*get_cleaned_delimiter(char *delimiter)
{
	char	*cleaned;
	int		i;
	int		j;
	int		len;
	char	quote_char;

	len = ft_strlen(delimiter);
	cleaned = malloc(len + 1);
	if (!cleaned)
		return (NULL);
	
	i = 0;
	j = 0;
	quote_char = '\0';
	
	while (delimiter[i])
	{
		if ((delimiter[i] == '\'' || delimiter[i] == '"') && quote_char == '\0')
		{
			quote_char = delimiter[i];
			i++;
			continue;
		}
		if (delimiter[i] == quote_char)
		{
			quote_char = '\0';
			i++;
			continue;
		}
		cleaned[j++] = delimiter[i++];
	}
	cleaned[j] = '\0';
	return (cleaned);
}

static int	is_delimiter_quoted(char *delimiter)
{
	int	i;

	i = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static int	process_single_heredoc(t_redirect *redir, t_all *all)
{
	int		heredoc_pipe[2];
	char	*cleaned_delimiter;
	int		quoted;
	int		ret;

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe for heredoc");
		return (1);
	}

	cleaned_delimiter = get_cleaned_delimiter(redir->filename);
	if (!cleaned_delimiter)
	{
		handle_heredoc_error(heredoc_pipe, 1);
		return (1);
	}

	quoted = is_delimiter_quoted(redir->filename);
	ret = read_heredoc_input(heredoc_pipe[1], cleaned_delimiter, all, quoted);
	
	if (ret != 0)
	{
		free(cleaned_delimiter);
		return (handle_heredoc_error(heredoc_pipe, ret));
	}

	close(heredoc_pipe[1]);
	redir->fd = heredoc_pipe[0];
	free(cleaned_delimiter);
	return (0);
}

int	handle_all_heredocs_for_cmd(t_cmd *cmd, t_all *all)
{
	t_redirect	*redir;
	t_redirect	*last_heredoc;
	int			ret;

	// Find the last heredoc redirect
	redir = cmd->redirects;
	last_heredoc = NULL;
	
	while (redir)
	{
		if (redir->type == HEREDOC)
			last_heredoc = redir;
		redir = redir->next;
	}
	
	// If no heredoc found, return success
	if (!last_heredoc)
		return (0);

	// Process ALL heredocs but only keep the last one's fd
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			ret = process_single_heredoc(redir, all);
			if (ret != 0)
				return (ret);
			
			// If this is not the last heredoc, close its fd
			if (redir != last_heredoc && redir->fd > 0)
			{
				close(redir->fd);
				redir->fd = -1;
			}
		}
		redir = redir->next;
	}
	return (0);
}

void	cleanup_heredoc_fds(t_cmd *cmd)
{
	t_redirect	*redir;

	if (!cmd)
		return;
	
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == HEREDOC && redir->fd > 0)
		{
			close(redir->fd);
			redir->fd = -1;
		}
		redir = redir->next;
	}
}

void	cleanup_all_heredoc_fds(t_all *all)
{
	t_cmd	*cmd;

	if (!all || !all->cmd)
		return;
	
	cmd = all->cmd;
	while (cmd)
	{
		cleanup_heredoc_fds(cmd);
		cmd = cmd->next;
	}
}

int	check_here_flag(t_card *card, char *eof)
{
	t_card	*current;

	current = card;
	while (current)
	{
		if (current->here_flag == 1 && ft_strcmp(current->value, eof) == 0)
			return (0);
		current = current->next;
	}
	return (1);
}
