/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:43 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/19 09:43:44 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>

static int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	current = env;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_node, t_all *all)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(env_node->key, "=", all);
	result = ft_strjoin(temp, env_node->value, all);
	free(temp);
	return (result);
}

char	**list_to_envp(t_all *all)
{
	t_env	*current;
	char	**envp;
	int		count;
	int		i;

	count = count_env_vars(all->env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
	{
		reset_all(all, EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}
	current = all->env;
	i = 0;
	while (i < count)
	{
		envp[i] = create_env_string(current, all);
		current = current->next;
		i++;
	}
	envp[count] = NULL;
	return (envp);
}
