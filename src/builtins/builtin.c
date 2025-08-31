/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:52:55 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 12:01:53 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <string.h>

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		strcmp(cmd, "echo") == 0
		|| strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "pwd") == 0
		|| strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0
		|| strcmp(cmd, "env") == 0
	);
}

int	exec_builtin(t_all *all, t_cmd *cmd, int flag)
{
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(all, cmd));
	else if (strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(all, cmd));
	else if (strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(all, cmd));
	else if (strcmp(cmd->args[0], "export") == 0)
		return (ft_export(all, cmd));
	else if (strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(all, cmd));
	else if (strcmp(cmd->args[0], "env") == 0)
		return (ft_env(all, cmd));
	else if (strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(all, cmd, flag));
	return (1);
}
