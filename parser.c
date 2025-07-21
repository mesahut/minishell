#include "lexer.h"

int redir_counter(t_all *all)
{
    t_card *current_card;
    int count;

    count = 0;
    current_card = all->card;
    while (current_card != NULL || current_card->type != PIPE)
    {
        if (current_card->type <= R_IN || current_card->type >= R_APPEND)
            count++;
        current_card = current_card->next;
    }
    return (count);
}

int args_counter(t_all *all)
{
    t_card *current_card;
    int count;

    count = 0;
    current_card = all->card;
    while (current_card != NULL && current_card->type != PIPE)
    {
        if (current_card->type == WORD)
            count++;
        current_card = current_card->next;
    }
    return (count);
}

t_cmd   *init_cmd(t_cmd *head_cmd, t_all *all)
{
    t_cmd   *cmd;
    int     arg_count;

    arg_count = args_counter(all);
    cmd = (t_cmd *)safe_malloc(all->collector, sizeof(t_cmd));
    cmd->args = (char **)safe_malloc(all->collector, sizeof(char *) * (arg_count + 1));
    cmd->fd_in = 0;
    cmd->fd_out = 1;
    cmd->redirect_count = redir_counter(all);
    cmd->redirects = (t_redirect *)safe_malloc(all->collector, sizeof(t_redirect) * redir_counter(all));
    cmd->prev = head_cmd;
    cmd->next = NULL;
    cmd->args[arg_count] = NULL;
    all->collector->value = cmd;
    return (cmd);
}

void    put_node(t_cmd **head_cmd, t_cmd *new_cmd)
{
    t_cmd *temp;
    if (*head_cmd == NULL)
    {
        *head_cmd = new_cmd;
    }
    else
    {
        temp = *head_cmd;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_cmd;
        new_cmd->prev = temp;
    }
}
set_cmd(t_all *all, t_cmd *current_cmd)
{
    t_card *current_card;
    int i;

    current_card = all->card;
    i = 0;
    while (current_card != NULL && current_card->type != PIPE)
    {
        if (current_card->type == WORD)
        {
            current_cmd->args[i] = current_card->value;
            i++;
        }
        else if (current_card->type <= R_IN || current_card->type >= R_APPEND)
        {
            current_cmd->
        }
        current_card = current_card->next;
    }
}

void parser(t_all *all)
{
    t_cmd *head_cmd;
    t_cmd *current_cmd;

    head_cmd = NULL;
    current_cmd = init_cmd(head_cmd ,all);
    put_node(&head_cmd, current_cmd);
    set_cmd(all, current_cmd);
}
