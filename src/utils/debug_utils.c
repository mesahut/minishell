/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:00:00 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/19 09:04:45 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

void	print_cmd(t_cmd *cmd)
{
	t_cmd		*current;
	t_redirect	*redir;
	int			i;

	current = cmd;
	while (current)
	{
		i = 0;
		while (i < current->args_count)
		{
			if (current->args[i])
				printf("%s ", current->args[i]);
			i++;
		}
		printf("\n");
		redir = current->redirects;
		while (redir)
		{
			if (redir->filename)
				printf("Redirect: %d %s\n", redir->type, redir->filename);
			else
				printf("Redirect: %d NULL\n", redir->type);
			redir = redir->next;
		}
		current = current->next;
	}
}
