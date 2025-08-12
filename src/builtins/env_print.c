/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:15:23 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/12 18:40:16 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cmp_env(const void *a, const void *b)
{
	const char	*s1;
	const char	*s2;

	s2 = *(const char **)b;
	s1 = *(const char **)a;
	return (strcmp(s1, s2));
}

int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	**get_sorted_keys(t_all *all)
{
	int		size;
	char	**keys;
	t_env	*current;
	int		i;

	current = all->env;
	i = 0;
	size = env_list_size(all->env);
	keys = malloc(sizeof(char *) * (size + 1));
	if (!keys)
		reset_all(all, 12);
	while (current)
	{
		keys[i++] = strdup(current->key);
		current = current->next;
	}
	keys[i] = NULL;
	qsort(keys, size, sizeof(char *), cmp_env);
	return (keys);
}

void	print_sorted_env(t_all *all)
{
	char	**keys;
	int		j;
	t_env	*node;

	keys = get_sorted_keys(all);
	if (!keys)
		return ;
	j = 0;
	while (j < env_list_size(all->env))
	{
		node = find_env_by_key(all->env, keys[j]);
		if (node)
		{
			if (node->value && node->value[0] != '\0')
				printf("declare -x %s=\"%s\"\n",
					node->key, node->value);
			else
				printf("declare -x %s\n", node->key);
		}
		free(keys[j++]);
	}
	free(keys);
}
