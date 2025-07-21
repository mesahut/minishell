/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:50:51 by asezgin           #+#    #+#             */
/*   Updated: 2025/07/21 12:02:29 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <unistd.h>     // fork, execve
#include <sys/wait.h>   // waitpid, WIFEXITED
#include <stdlib.h>     // exit
#include <stdio.h>      // perror, fprintf

char *path_find(char *cmd)
{
	char *new_path;

	new_path = ft_strjoin("/usr/bin/", cmd);
	return (new_path);
}
int exec_external_cmd(char *path, char **args, char **envp)
{
    pid_t pid = fork();
    int status = 0;

    if (pid == 0)
    {
        execve(path, args, envp);
        perror("execve failed");
        exit(1);
    }
    else if (pid > 0)
    {
        if (waitpid(pid, &status, 0) == -1)
            perror("waitpid failed");
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return 1;
    }
    else
    {
        perror("fork failed");
        return 1;
    }
}

void	exec(t_all *all)
{
	char *env[] = {NULL};
	t_cmd	*current_cmd;

	if (!all || !all->cmd)
		return ;
	current_cmd = all->cmd;
	while (current_cmd != NULL)
	{
		if (current_cmd->args[0])
		{
			if (is_builtin(current_cmd->args[0]))
				all->exit_status = exec_builtin(all, current_cmd);
			else
				exec_external_cmd(path_find(current_cmd->args[0]), current_cmd->args, env);			
		}
		current_cmd = current_cmd->next;
	}
}
