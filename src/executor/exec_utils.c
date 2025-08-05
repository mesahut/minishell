/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:00:00 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/05 17:54:24 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

char	*path_find(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (ft_strdup(cmd));
	return (ft_strjoin("/usr/bin/", cmd));
}

char	**list_to_envp(t_env *env)
{
	t_env	*current;
	int		count;
	int		i;

	current = env;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	char **envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = env;
	i = 0;
	while (i < count)
	{
		char *temp = ft_strjoin(current->key, "=");
		envp[i] = ft_strjoin(temp, current->value);
		free(temp);
		current = current->next;
		i++;
	}
	envp[count] = NULL;
	return (envp);
}

void	exec_external_cmd(char *path, char **args, t_all *all)
{
	char	**envp;
	int		i;

	envp = list_to_envp(all->env);
	execve(path, args, envp);
	printf("%s: command not found\n", args[0]);
	// Free the path if it was dynamically allocated by path_find
	// path_find always allocates memory, so we should always free it
	if (path)
		free(path);
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
	reset_all(all);
	free_env(all->env);
	rl_clear_history();
	exit(1);
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
		}
		i++;
	}
	return (str);
}

int	check_here_flag(t_card *card, char *eof)
{
	t_card	*current;

	current = card;
	while (current)
	{
		if (current->here_flag == 1 && strcmp(current->value, eof) == 0)
			return (0);
		current = current->next;
	}
	return (1);
}

