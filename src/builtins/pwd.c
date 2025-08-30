/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 08:40:00 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/30 20:02:30 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

int	ft_pwd(t_all *all, t_cmd *cmd)
{
	char	cwd[1024];
	int		ok;

	(void)all;
	(void)cmd;
	getcwd(cwd, sizeof(cwd));
	ok = access(cwd, X_OK);
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else if (ok == -1)
	{
		perror("error: ");
		return (1);
	}
	return (0);
}
