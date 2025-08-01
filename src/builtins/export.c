/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:13:21 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/01 19:18:57 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	update_env_value(t_env *env, const char *key, const char *value)
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

static void	append_env_node(t_all *all, const char *key, const char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = safe_malloc(all->collector, sizeof(t_env));
	new_node->key = strdup(key);
	new_node->value = strdup(value);
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

static int	parse_export_arg(t_all *all, const char *arg)
{
	const char	*equal_sign;
	char		*arg_copy;
	char		*equal_pos;

	/*
	if (arg && ft_isdigit(arg[0]))
	{
		fprintf(stderr, "export: `%s`: not a valid identifier\n", arg);
		return (1);
	}
	*/
	equal_sign = strchr(arg, '=');
	if (!equal_sign || equal_sign == arg)
	{
		fprintf(stderr, "export: `%s`: not a valid identifier\n", arg);
		return (1);
	}
	arg_copy = ft_strdup(arg);
	if (!arg_copy)
		return (1);
	equal_pos = strchr(arg_copy, '=');
	*equal_pos = '\0';
	add_or_update_env(all, arg_copy, equal_pos + 1);
	free(arg_copy);
	return (0);
}

int	ft_export(t_all *all, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
	{
		print_sorted_env(all->env);
		return (0);
	}
	while (cmd->args[i])
	{
		parse_export_arg(all, cmd->args[i]);
		i++;
	}
	return (0);
}
