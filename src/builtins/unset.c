/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:31:33 by asezgin           #+#    #+#             */
/*   Updated: 2025/09/01 16:27:50 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	remove_env_key(t_all *all, const char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = all->env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				all->env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset(t_all *all, t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		remove_env_key(all, cmd->args[i]);
		i++;
	}
	return (0);
}
