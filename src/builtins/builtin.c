/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:52:55 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 18:23:40 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <string.h>

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_all *all, t_cmd *cmd, int flag)
{
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(all, cmd));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(all, cmd));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(all, cmd));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(all, cmd));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(all, cmd));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(all, cmd));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(all, cmd, flag));
	return (1);
}
