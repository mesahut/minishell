/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:13:21 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/19 16:02:36 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(const char *identifier)
{
	int	i;

	if (!identifier || !*identifier)
		return (0);
	if (ft_isdigit(identifier[0]) || identifier[0] == '=')
		return (0);
	i = 0;
	while (identifier[i] && identifier[i] != '=')
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_with_value(t_all *all, const char *arg)
{
	char	*arg_copy;
	char	*equal_pos;
	char	*value;

	arg_copy = ft_strdup(arg, all);
	if (!arg_copy)
		return (1);
	equal_pos = strchr(arg_copy, '=');
	value = equal_pos + 1;
	*equal_pos = '\0';
	add_or_update_env(all, arg_copy, value);
	free(arg_copy);
	return (0);
}

static int	handle_export_without_value(t_all *all, const char *arg)
{
	char	*arg_copy;

	arg_copy = ft_strdup(arg, all);
	if (!arg_copy)
		return (1);
	append_env_node(all, arg_copy, NULL);
	free(arg_copy);
	return (0);
}

static int	parse_export_arg(t_all *all, const char *arg)
{
	const char	*equal_sign;

	if (!arg)
		return (1);
	if (!is_valid_identifier(arg))
	{
		fprintf(stderr, "not a valid identifier\n");
		all->exit_status = 1;
		return (1);
	}
	equal_sign = strchr(arg, '=');
	if (equal_sign)
		return (handle_export_with_value(all, arg));
	else
		return (handle_export_without_value(all, arg));
}

int	ft_export(t_all *all, t_cmd *cmd)
{
	int	i;
	int	has_error;

	i = 1;
	has_error = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(all);
		return (0);
	}
	while (cmd->args[i])
	{
		if (parse_export_arg(all, cmd->args[i]))
			has_error = 1;
		i++;
	}
	return (has_error);
}
