/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:50:51 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/04 13:04:12 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <unistd.h>     // fork, execve
#include <sys/wait.h>   // waitpid, WIFEXITED
#include <stdlib.h>     // exit
#include <stdio.h>      // perror, fprintf
#include <fcntl.h>
#include <readline/readline.h>

char *path_find(char *cmd)
{
    if (!cmd)
        return NULL;
    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
        return (ft_strdup(cmd));
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

void exec_external_cmd(char *path, char **args, t_all *all)
{
    execve(path, args, list_to_envp(all->env));
    printf("%s: command not found\n", args[0]);
    reset_all(all);
    //envleri sil dayı
    rl_clear_history();
    exit(1); // execve başarısız olursa çıkış yap
}

char    *here_expand(char *str, t_all *all)
{
    int i;
    char open_quote;

    i = 0;
    open_quote = '\0';
    while (str[i])
	{
		open_quote = is_char_quote(str[i], open_quote);
		if(str[i] == '$' && str[i + 1] != '\0')
		{
			str = found_dollar(str, i, all);
		}
		i++;
	}
    return (str);
}

int check_here_flag(t_card *card, char *eof)
{
    t_card *current = card;
    
    while (current)
    {
        if (current->here_flag == 1 && strcmp(current->value, eof) == 0)
            return (0);
        current = current->next;
    }
    return (1);
}

void handle_redirections(t_cmd *cmd, t_all *all)
{
    t_redirect *redir = cmd->redirects;

    while (redir)
    {
        if (redir->type == R_OUT)
        {
            redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (redir->fd < 0)
            {
                perror(redir->filename);
                return;
            }
            if (dup2(redir->fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 for output");
                close(redir->fd);
                return;
            }
            close(redir->fd);
        }
        else if (redir->type == R_APPEND)
        {
            redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (redir->fd < 0)
            {
                perror(redir->filename);
                return;
            }
            if (dup2(redir->fd, STDOUT_FILENO) == -1)
            {
                perror("dup2 for append");
                close(redir->fd);
                return;
            }
            close(redir->fd);
        }
        else if (redir->type == R_IN)
        {
            redir->fd = open(redir->filename, O_RDONLY);
            if (redir->fd < 0)
            {
                perror(redir->filename);
                return;
            }
            if (dup2(redir->fd, STDIN_FILENO) == -1)
            {
                perror("dup2 for input");
                close(redir->fd);
                return;
            }
            close(redir->fd);
        }
        else if (redir->type == HEREDOC)
        {
            int pipefd[2];
            if (pipe(pipefd) == -1)
            {
                perror("pipe for heredoc");
                return;
            }

            char *line;
            while ((line = readline("> ")) != NULL)
            {
                if (strcmp(line, redir->filename) == 0)
                {
                    free(line);
                    break;
                }
                if(check_here_flag(all->card, redir->filename))
                    line = here_expand(line, all);
                write(pipefd[1], line, ft_strlen(line));
                write(pipefd[1], "\n", 1);
                free(line);
            }
            close(pipefd[1]);             // yazmca ucunu kapat (önemli)
            redir->fd = pipefd[0];        // okuma ucunu stdin'e bağlayacağız

            if (dup2(redir->fd, STDIN_FILENO) == -1)
            {
                perror("dup2 for heredoc");
                close(redir->fd);
                return;
            }
            close(redir->fd);             // artık stdin'e bağlı olduğu için kapatabiliriz
        }

        redir = redir->next;
    }
}


void exec(t_all *all)
{
    t_cmd *cmd = all->cmd;
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;
    int saved_stdin = -1;
    int saved_stdout = -1;

    while (cmd)
    {
        if (cmd->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(1);
        }
        if (is_builtin(cmd->args[0]) && cmd->next == NULL)
        {
            // Built-in ve pipeline'da değilse fork açma, direkt shell içinde çalıştır
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            
            // Save original file descriptors before redirections
            if (cmd->redirects)
            {
                saved_stdin = dup(STDIN_FILENO);
                saved_stdout = dup(STDOUT_FILENO);
                handle_redirections(cmd, all);
            }
            all->exit_status = exec_builtin(all, cmd);
            
            // Restore original file descriptors after builtin execution
            if (saved_stdin != -1)
            {
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdin);
            }
            if (saved_stdout != -1)
            {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
            break;
        }
        else
        {
            // Fork aç ve child içinde çalıştır (built-in ya da external)
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
                    handle_redirections(cmd, all);

                if (cmd->next)
                {
                    close(pipefd[0]);
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                }

                if (is_builtin(cmd->args[0]))
                {
                    exit(exec_builtin(all, cmd));
                }
                else
                {                    
                    exec_external_cmd(path_find(cmd->args[0]), cmd->args, all);
                }

                exit(1); // exec başarısızsa child burada ölür
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
}

