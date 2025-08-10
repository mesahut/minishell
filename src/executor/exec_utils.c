/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:23:40 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/10 10:46:16 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char *tmp = ft_strjoin(s1, s2);
	char *result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

char	*path_find(char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));

	// PATH environment variable (kendi env listenden alabilirsin)
	path_env = getenv("PATH");  // Alternatif: get_env_value("PATH", all->env);
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);  // Üç stringi birleştiren bir yardımcı fonksiyon
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
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
	if (args[0][0] == '/')
		printf("%s: No such file or directory\n", args[0]);
	else if (args[0][0] == '>' || args[0][0] == '<')
		printf("%s: syntax error\n", args[0]);
	else 
		printf("%s: command not found\n", args[0]);
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

