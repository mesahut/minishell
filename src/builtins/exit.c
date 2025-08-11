/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:49:22 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/11 21:35:24 by mayilmaz         ###   ########.fr       */
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
	rl_clear_history();
	printf("%s\n", "exit");
	exit(all->exit_status);
}
