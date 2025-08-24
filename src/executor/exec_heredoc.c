/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:43:50 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/24 19:59:00 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../include/minishell.h"

static int  read_heredoc_input(int write_fd, char *eof, t_all *all, int is_last)
{
    char    *line;

	is_last = 0;
    while (1)
    {
        /* Her satır için heredoc promptunu yazdır */
        write(STDOUT_FILENO, "> ", 2);
        /* Boş prompt ile readline çağırıyoruz; böylece kendi yazdığımız "> " promptu bozulmaz */
        line = readline("");
        if (!line)
            return (1); /* ctrl-D veya EOF durumunda hata döndürülür */

        /* Delimiter ile karşılaşırsak döngüden çık */
        if (strcmp(line, eof) == 0)
        {
            free(line); /* Sadece son here-doc bitiminde yeni satır */
            break;
        }
        /* Değişken genişletmesi gerekiyorsa */
        if (check_here_flag(all->card, eof))
            line = here_expand(line, all);

        /* Eğer yazılacak fd geçerli ise, içeriği pipe’a yazdır */
        if (write_fd >= 0)
        {
            write(write_fd, line, ft_strlen(line));
            write(write_fd, "\n", 1);
        }
        free(line);
    }
    return (0);
}

/* Tüm heredoc’ları işler; yalnızca sonuncusunun çıktısı komutun stdin’ine bağlanır */
static void heredoc_loop(t_all *all, char **heredocs, int pipefd[2])
{
    int i;

    i = 0;
    while (heredocs[i])
    {
        int is_last = (heredocs[i + 1] == NULL);
        int fd = is_last ? pipefd[1] : -1;
        read_heredoc_input(fd, heredocs[i], all, is_last);
        i++;
    }
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

/* heredoc redirect’lerini işler, yalnızca sonuncusunu stdin’e bağlar. */
int handle_heredoc_process(t_redirect *redir, t_all *all)
{
    int   heredoc_pipe[2];
    char **heredocs;

    heredocs = collect_heredocs(all);
    if (!heredocs)
        return (1);
    if (pipe(heredoc_pipe) == -1)
    {
        perror("pipe for heredoc");
        free(heredocs);
        return (1);
    }
    heredoc_loop(all, heredocs, heredoc_pipe);
    free(heredocs);
    close(heredoc_pipe[1]);           /* Yazma ucunu kapat */
    redir->fd = heredoc_pipe[0];      /* Okuma ucunu redirect’te sakla */
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
