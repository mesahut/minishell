/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:50 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/25 11:55:06 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/minishell.h"

static int  read_heredoc_input(int write_fd, char *eof, t_all *all)
{
    char    *line;
    extern int g_signal;

    while (1)
    {
        if (g_signal == SIGINT)
            return (130);
        write(STDOUT_FILENO, "> ", 2);
        line = readline("");
        if (g_signal == SIGINT)
        {
            if (line)
                free(line);
            return (130);
        }
        if (!line)
        {
            if (g_signal == SIGINT)
                return (130);
            write(STDERR_FILENO, "minishell: warning: here-document delimited by end-of-file (wanted `", 67);
            write(STDERR_FILENO, eof, ft_strlen(eof));
            write(STDERR_FILENO, "')\n", 3);
            return (1);
        }
        if (strcmp(line, eof) == 0)
        {
            free(line);
            break;
        }
        if (check_here_flag(all->card, eof))
            line = here_expand(line, all);
        if (write_fd >= 0)
        {
            write(write_fd, line, ft_strlen(line));
            write(write_fd, "\n", 1);
        }
        free(line);
    }
    return (0);
}

/* Tüm heredoc'ları işler; yalnızca sonuncusunun çıktısı komutun stdin'ine bağlanır */
static int heredoc_loop(t_all *all, char **heredocs, int pipefd[2])
{
    int i;
    int ret;

    i = 0;
    while (heredocs[i])
    {
        int is_last = (heredocs[i + 1] == NULL);
        int fd = is_last ? pipefd[1] : -1;
            
        ret = read_heredoc_input(fd, heredocs[i], all);
        if (ret != 0)
            return (ret);
        i++;
    }
    return (0);
}


/* Tüm heredoc redirect’lerini sayar ve bir char ** dizi döndürür.
   Dizi sonuna NULL eklenir; her eleman orijinal filename’lere işaret eder. */
static char **collect_heredocs(t_all *all)
{
    t_redirect  *tmp;
    char        **heredocs;
    int          count;
    int          i;

    /* redirect listesini yürüyerek kaç adet heredoc olduğunu bulalım */
    tmp = all->cmd->redirects;
    count = 0;
    while (tmp)
    {
        if (tmp->type == HEREDOC)
            count++;
        tmp = tmp->next;
    }
    heredocs = malloc(sizeof(char *) * (count + 1));
    if (!heredocs)
        return (NULL);
    /* heredoc isimlerini diziye kopyalayalım */
    tmp = all->cmd->redirects;
    i = 0;
    while (tmp)
    {
        if (tmp->type == HEREDOC)
            heredocs[i++] = tmp->filename;
        tmp = tmp->next;
    }
    heredocs[i] = NULL;
    return (heredocs);
}

/* heredoc redirect'lerini işler, yalnızca sonuncusunu stdin'e bağlar. */
int handle_heredoc_process(t_redirect *redir, t_all *all)
{
    int   heredoc_pipe[2];
    char **heredocs;
    int   ret;

    heredocs = collect_heredocs(all);
    if (!heredocs)
        return (1);
    if (pipe(heredoc_pipe) == -1)
    {
        perror("pipe for heredoc");
        free(heredocs);
        return (1);
    }
    signal_switch(3); // Set heredoc signal handler
    ret = heredoc_loop(all, heredocs, heredoc_pipe);
    signal_switch(1); // Reset to prompt signal handler
    if (ret != 0)
    {
        free(heredocs);
        close(heredoc_pipe[0]);
        close(heredoc_pipe[1]);
        // Reopen stdin if it was closed by signal
        if (ret == 130)
        {
            freopen("/dev/tty", "r", stdin);
        }
        return (ret);
    }
    free(heredocs);
    close(heredoc_pipe[1]);           /* Yazma ucunu kapat */
    redir->fd = heredoc_pipe[0];      /* Okuma ucunu redirect'te sakla */
    if (dup2(redir->fd, STDIN_FILENO) == -1)
    {
        perror("dup2 for heredoc");
        close(redir->fd);
        return (1);
    }
    close(redir->fd);
    return (0);
}

/* here_flag kontrolleri aynı kalır */
int check_here_flag(t_card *card, char *eof)
{
    t_card  *current;

    current = card;
    while (current)
    {
        if (current->here_flag == 1 && strcmp(current->value, eof) == 0)
            return (0);
        current = current->next;
    }
    return (1);
}
