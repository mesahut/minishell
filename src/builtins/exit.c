/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:49:22 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/04 11:37:21 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(t_all *all, t_cmd *cmd)
{
	int	status;

	(void)all;
	if (cmd->args[1])
	{
		status = atoi(cmd->args[1]);
		exit(status);
	}
	reset_all(all);
	free_env(all->env);
	rl_clear_history();
	printf("%s\n", "exit");
	exit(all->exit_status);
}
