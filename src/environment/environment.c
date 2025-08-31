/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:26:24 by asezgin           #+#    #+#             */
/*   Updated: 2025/07/28 11:26:24 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

void	create_env(char **env_line, t_all *all)
{
	t_env	*new_env;
	t_env	*current;

	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		reset_all(all, EXIT_ALLOC_FAIL);
	new_env->key = ft_strdup(env_line[0], all);
	if (env_line[1])
		new_env->value = ft_strdup(env_line[1], all);
	else
		new_env->value = NULL;
	new_env->next = NULL;
	if (all->env == NULL)
		all->env = new_env;
	else
	{
		current = all->env;
		while (current->next != NULL)
			current = current->next;
		current->next = new_env;
	}
}

void	put_env(t_all *all, char **env_list)
{
	int		i;
	char	*key;
	char	*value;
	char	*eq_pos;

	i = 0;
	while (env_list[i])
	{
		eq_pos = ft_strchr(env_list[i], '=');
		if (eq_pos)
		{
			key = ft_substr(env_list[i], 0, eq_pos - env_list[i], all);
			value = ft_strdup(eq_pos + 1, all);
		}
		else
		{
			key = ft_strdup(env_list[i], all);
			value = NULL;
		}
		create_env((char *[3]){key, value, NULL}, all);
		free(key);
		free(value);
		i++;
	}
}
