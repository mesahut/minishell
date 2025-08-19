/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:46:46 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/19 15:49:22 by asezgin          ###   ########.fr       */
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
			if (current->value && current->value[0] != '\0')
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

int	update_env_value(t_env *env, const char *key, const char *value)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = strdup(value);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	append_env_node(t_all *all, const char *key, const char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		reset_all(all, EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}
	new_node->key = ft_strdup(key, all);
	new_node->value = ft_strdup(value, all);
	new_node->next = NULL;
	if (!all->env)
	{
		all->env = new_node;
		return ;
	}
	current = all->env;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	add_or_update_env(t_all *all, const char *key, const char *value)
{
	if (!update_env_value(all->env, key, value))
		append_env_node(all, key, value);
}
