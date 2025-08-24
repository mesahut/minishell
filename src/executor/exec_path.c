/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:44:19 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/24 15:16:09 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>

static char	*ft_strjoin3(char *s1, char *s2, char *s3, t_all *all)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2, all);
	result = ft_strjoin(tmp, s3, all);
	free(tmp);
	return (result);
}

static char	*check_direct_path(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (cmd);
		else
			return (NULL);
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd, t_all *all)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd, all);
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

char	*path_find(char *cmd, t_all *all)
{
	char	**paths;
	char	*path_env;
	char	*direct_path;

	if ((cmd[0] == '/' || cmd[0] == '.') && cmd[1] == '\0')
		return (NULL);
	direct_path = check_direct_path(cmd);
	if (direct_path)
		return (direct_path);
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':', all);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd, all));
}
