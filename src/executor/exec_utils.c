/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:44:33 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/20 11:18:02 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <fcntl.h>

static int	print_error_msg(char *cmd)
{
	if (cmd[0] == '/')
	{
		printf("%s: No such file or directory\n", cmd);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	else if (cmd[0] == '>' || cmd[0] == '<')
	{
		printf("%s: syntax error\n", cmd);
		return (EXIT_MISUSE);
	}
	else
	{
		printf("%s: command not found\n", cmd);
		return (EXIT_COMMAND_NOT_FOUND);
	}
}

static void	free_envp_array(char **envp)
{
	int	i;

	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
}

void	exec_external_cmd(char *path, char **args, t_all *all)
{
	char	**envp;
	int		exit_code;

	envp = list_to_envp(all);
	execve(path, args, envp);
	exit_code = print_error_msg(args[0]);
	if (path)
		free(path);
	free_envp_array(envp);
	reset_all(all, 0);
	rl_clear_history();
	exit(exit_code);
}

char	*here_expand(char *str, t_all *all)
{
	int		i;
	char	open_quote;

	i = 0;
	open_quote = '\0';
	while (str[i])
	{
		open_quote = is_char_quote(str[i], open_quote);
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			str = found_dollar(str, i, all);
			i = -1;
		}
		i++;
	}
	return (str);
}
