/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:49:22 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/30 20:02:05 by asezgin          ###   ########.fr       */
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
	while (str[i] && is_space(str[i]))
			i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i] && !is_space(str[i]))
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i])
		return (0);
	return (1);
}

int	ft_atoi(char *str)
{
	int			i;
	long int	number;
	int			sign;

	i = 0;
	number = 0;
	sign = 1;
	while(str[i] && is_space(str[i]))
		i++;
	if (str[i] == 45 || str[i] == 43)
	{
		if (str[i] == 45)
			sign *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		number *= 10;
		number += str[i] - 48;
		i++;
	}
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i])
		return (0);
	number *= sign;
	return (number);
}

static int	is_numeric_overflow(char *str)
{
	long	num;

	if (!is_valid_number(str))
		return (1);
	num = ft_atoi(str);
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
	if (is_numeric_overflow(cmd->args[1]))
	{
		printf("minishell: exit: %snumeric argument required\n", cmd->args[1]);
		all->exit_status = 2;
		exit_with_cleanup(all);
	}
	exit_code = (unsigned char)ft_atoi(cmd->args[1]);
	all->exit_status = exit_code;
	exit_with_cleanup(all);
	return (0);
}
