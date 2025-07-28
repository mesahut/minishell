/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 08:39:58 by asezgin           #+#    #+#             */
/*   Updated: 2025/07/28 11:12:23 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*resolve_cd_path(t_cmd *cmd)
{
	if (!cmd->args[1] || strcmp(cmd->args[1], "~") == 0)
		return (getenv("HOME"));
	return (cmd->args[1]);
}

static int	print_cd_home_error(void)
{
	fprintf(stderr, "cd: HOME not set\n");
	return (1);
}

static void	print_cd_path_error(char *path)
{
	fprintf(stderr, "cd: %s: No such file or directory\n", path);
}

static void	update_env_pwd(t_all *all, char *oldpwd)
{
	char	cwd[1024];

	if (oldpwd)
		add_or_update_env(all, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		add_or_update_env(all, "PWD", cwd);
	free(oldpwd);
}

int	ft_cd(t_all *all, t_cmd *cmd)
{
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	path = resolve_cd_path(cmd);
	if (!path)
		return (print_cd_home_error());
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("cd: getcwd"), 1);
	oldpwd = strdup(cwd);
	if (chdir(path) != 0)
		return (print_cd_path_error(path), free(oldpwd), 1);
	update_env_pwd(all, oldpwd);
	return (0);
}
