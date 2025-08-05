/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:47:00 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/05 17:51:10 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_fds(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (fds[i] != -1)
		{
			close(fds[i]);
			fds[i] = -1;
		}
		i++;
	}
}

void	cleanup_pipe(int pipefd[2])
{
	if (pipefd[0] != -1)
	{
		close(pipefd[0]);
		pipefd[0] = -1;
	}
	if (pipefd[1] != -1)
	{
		close(pipefd[1]);
		pipefd[1] = -1;
	}
}

void	cleanup_redirects(t_redirect *redirects)
{
	t_redirect	*current;

	current = redirects;
	while (current)
	{
		if (current->fd != -1)
		{
			close(current->fd);
			current->fd = -1;
		}
		current = current->next;
	}
}
