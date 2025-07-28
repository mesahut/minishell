/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:15:23 by asezgin           #+#    #+#             */
/*   Updated: 2025/07/28 10:56:53 by asezgin          ###   ########.fr       */
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

char	**env_to_array(t_env *env_list)
{
	char	**arr;
	int		i;
	char	*tmp;

	i = 0;
	arr = malloc(sizeof(char *) * (env_list_size(env_list) + 1));
	if (!arr)
		return (NULL);
	while (env_list)
	{
		if (env_list->value)
		{
			tmp = malloc(strlen(env_list->key) + strlen(env_list->value) + 2);
			sprintf(tmp, "%s=%s", env_list->key, env_list->value);
		}
		else
		{
			tmp = strdup(env_list->key);
		}
		arr[i++] = tmp;
		env_list = env_list->next;
	}
	arr[i] = NULL;
	return (arr);
}

static char	**get_sorted_keys(t_env *env_list)
{
	int		size;
	char	**keys;
	t_env	*current;
	int		i;

	current = env_list;
	i = 0;
	size = env_list_size(env_list);
	keys = malloc(sizeof(char *) * (size + 1));
	if (!keys)
		return (NULL);
	while (current)
	{
		keys[i++] = strdup(current->key);
		current = current->next;
	}
	keys[i] = NULL;
	qsort(keys, size, sizeof(char *), cmp_env);
	return (keys);
}

void	print_sorted_env(t_env *env_list)
{
	char	**keys;
	int		j;
	t_env	*node;

	keys = get_sorted_keys(env_list);
	if (!keys)
		return ;
	j = 0;
	while (j < env_list_size(env_list))
	{
		node = find_env_by_key(env_list, keys[j]);
		if (node)
		{
			if (node->value)
				printf("declare -x %s=\"%s\"\n",
					node->key, node->value);
			else
				printf("declare -x %s\n", node->key);
		}
		free(keys[j++]);
	}
	free(keys);
}
