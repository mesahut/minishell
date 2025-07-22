/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:50:51 by asezgin           #+#    #+#             */
/*   Updated: 2025/07/22 15:51:58 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include <unistd.h>     // fork, execve
#include <sys/wait.h>   // waitpid, WIFEXITED
#include <stdlib.h>     // exit
#include <stdio.h>      // perror, fprintf
#include <fcntl.h>

char *path_find(char *cmd)
{
    if (cmd[0] == '.' && cmd[1] == '/')
    {
        return (cmd);
    }
    else
        return (ft_strjoin("/usr/bin/", cmd));
}

char **list_to_envp(t_env *env)
{
    t_env *current = env;
    int count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }

    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;

    current = env;
    for (int i = 0; i < count; i++)
    {
        envp[i] = ft_strjoin(current->key, "=");
        envp[i] = ft_strjoin(envp[i], current->value);
        current = current->next;
    }
    envp[count] = NULL;

    return envp;
}

void exec_external_cmd(char *path, char **args, t_env *envp)
{
    execve(path, args, list_to_envp(envp));
    perror("execve failed");
    exit(1);
}

void handle_redirections(t_cmd *cmd)
{
    t_redirect *redir = cmd->redirects;

    while (redir)
    {
        if (redir->type == R_OUT)
        {
            redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (redir->fd < 0)
                perror("redirect >");
            else
                dup2(redir->fd, STDOUT_FILENO);
        }
        else if (redir->type == R_APPEND)
        {
            redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (redir->fd < 0)
                perror("redirect >>");
            else
                dup2(redir->fd, STDOUT_FILENO);
        }
        else if (redir->type == R_IN)
        {
            redir->fd = open(redir->filename, O_RDONLY);
            if (redir->fd < 0)
                perror("redirect <");
            else
                dup2(redir->fd, STDIN_FILENO);
        }
        else if (redir->type == HEREDOC)
        {
            // HEREDOC işleme kodu buraya gelir (temp dosya ya da pipe üzerinden)
            // Bu daha karmaşık bir işlem, istersen ayrıca açıklayabilirim
        }

        if (redir->fd >= 0)
            close(redir->fd); // Açık dosya descriptor'ları kapat
        redir = redir->next;
    }
}

void exec(t_all *all)
{
    t_cmd *cmd = all->cmd;
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;

    while (cmd)
    {
        if (cmd->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (cmd->redirects)
                handle_redirections(cmd);

            if (cmd->next)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            if (is_builtin(cmd->args[0]))
                exit(exec_builtin(all, cmd));
            else
            {
                exec_external_cmd(path_find(cmd->args[0]), cmd->args, all->env);
            }
                

            // exec_external_cmd başarısız olursa buraya gelir
            exit(1);
        }
        else
        {
            // Parent process
            if (prev_fd != -1)
                close(prev_fd);

            if (cmd->next)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
            else if (cmd->next == NULL && prev_fd != -1)
            {
                close(prev_fd);
            }

            waitpid(pid, &all->exit_status, 0);
            cmd = cmd->next;
        }
    }
}
