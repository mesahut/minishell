/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:49:22 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/29 16:56:05 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <limits.h>

static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	is_numeric_overflow(const char *str)
{
	long	num;

	if (!is_valid_number(str))
		return (1);
	num = ft_atol(str);
	if (num > LONG_MAX || num < LONG_MIN)
		return (1);
	return (0);
}

static void	exit_with_cleanup(t_all *all)
{
	int	n;

	n = all->exit_status;
	clean_malloc(all);
	if (all->env)
	{
		free_env(all->env);
		all->env = NULL;
	}
	rl_clear_history();
	exit(n);
}

int	ft_exit(t_all *all, t_cmd *cmd)
{
	int	exit_code;

	printf("exit\n");
	if (!cmd->args[1])
	{
		exit_with_cleanup(all);
	}
	if (cmd->args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		all->exit_status = 1;
		return (1);
	}
	if (is_numeric_overflow(cmd->args[1]) || !is_valid_number(cmd->args[1]))
	{
		fprintf(stderr, "numeric argument required\n");
		all->exit_status = 2;
		exit_with_cleanup(all);
	}
	exit_code = (unsigned char)ft_atol(cmd->args[1]);
	all->exit_status = exit_code;
	exit_with_cleanup(all);
	return (0);
}
