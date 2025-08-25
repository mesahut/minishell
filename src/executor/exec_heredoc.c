/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:50 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/25 15:02:58 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/minishell.h"

static char	**collect_heredocs(t_all *all)
{
	t_redirect	*tmp;
	char		**heredocs;
	int			count;
	int			i;

	count = 0;
	tmp = all->cmd->redirects;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			count++;
		tmp = tmp->next;
	}
	heredocs = (char **)safe_malloc(all, sizeof(char *) * (count + 1));
	i = 0;
	tmp = all->cmd->redirects;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			heredocs[i++] = tmp->filename;
		tmp = tmp->next;
	}
	heredocs[i] = NULL;
	return (heredocs);
}

static int	handle_heredoc_error(int heredoc_pipe[2], int ret)
{
	close(heredoc_pipe[0]);
	close(heredoc_pipe[1]);
	if (ret == 130)
		freopen("/dev/tty", "r", stdin);
	return (ret);
}

static int	setup_heredoc_stdin(t_redirect *redir, int heredoc_pipe[2])
{
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

int	handle_heredoc_process(t_redirect *redir, t_all *all)
{
	int		heredoc_pipe[2];
	char	**heredocs;
	int		ret;

	heredocs = collect_heredocs(all);
	if (!heredocs)
		return (1);
	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe for heredoc");
		return (1);
	}
	signal_switch(3);
	ret = heredoc_loop(all, heredocs, heredoc_pipe);
	signal_switch(1);
	if (ret != 0)
		return (handle_heredoc_error(heredoc_pipe, ret));
	free(heredocs);
	close(heredoc_pipe[1]);
	return (setup_heredoc_stdin(redir, heredoc_pipe));
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
