/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:50:51 by asezgin           #+#    #+#             */
/*   Updated: 2025/07/21 16:30:00 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <unistd.h>     // fork, execve, pipe, dup2
#include <sys/wait.h>   // waitpid, WIFEXITED
#include <stdlib.h>     // exit
#include <stdio.h>      // perror, fprintf

char *path_find(char *cmd)
{
    char *new_path;

    new_path = ft_strjoin("/usr/bin/", cmd);
    return (new_path);
}

void make_fork(t_cmd *cmd)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (cmd->fd_in != 0)
        {
            dup2(cmd->fd_in, STDIN_FILENO);
            close(cmd->fd_in);
        }
        if (cmd->fd_out != 1)
        {
            dup2(cmd->fd_out, STDOUT_FILENO);
            close(cmd->fd_out);
        }

        char *path = path_find(cmd->args[0]);
        if (!path)
        {
            fprintf(stderr, "Command not found: %s\n", cmd->args[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(path, cmd->args, NULL) == -1)
        {
            perror("Execution failed");
            free(path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (cmd->fd_in != 0)
            close(cmd->fd_in);
        if (cmd->fd_out != 1)
            close(cmd->fd_out);

        waitpid(pid, NULL, 0);
    }
}

void exec(t_all *all)
{
    t_cmd *current_cmd;
    int pipefd[2];

    if (!all || !all->cmd)
        return ;
    current_cmd = all->cmd;
    while (current_cmd && current_cmd->next)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        current_cmd->fd_out = pipefd[1];
        current_cmd->next->fd_in = pipefd[0];
        make_fork(current_cmd);
        current_cmd = current_cmd->next;
    }
    if (current_cmd)
    {
        if (current_cmd->fd_in == 0)
            current_cmd->fd_in = 0;
        if (current_cmd->fd_out == 0)
            current_cmd->fd_out = 1;

        make_fork(current_cmd);
    }
}
