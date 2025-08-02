/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:46:46 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/02 13:12:55 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(t_all *all, t_cmd *cmd)
{
	t_env	*current;

	current = all->env;
	if (cmd->args && !cmd->args[1])
	{
		while (current)
		{
			if (current->value)
				printf("%s=%s\n", current->key, current->value);
			current = current->next;
		}
	}
	else
	{
		fprintf(stderr, "env: `%s`: No such file or directory\n", cmd->args[1]);
		return (1);
	}
	return (0);
}

t_env	*find_env_by_key(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (!strcmp(env_list->key, key))
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}
