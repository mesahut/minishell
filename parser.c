#include "lexer.h"

int redir_counter(t_all *all)
{
    t_card *current_card;
    int count;

    count = 0;
    current_card = all->card;
    while (current_card != NULL && current_card->type != PIPE)
    {
        if (current_card->type == R_APPEND || current_card->type == R_OUT || 
            current_card->type == HEREDOC || current_card->type == R_IN)
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
        if (current_card->type == WORD || current_card->type == -1)
            count++;
        current_card = current_card->next;
    }
    return (count);
}

t_cmd   *init_cmd(t_cmd *head_cmd, t_all *all)
{
    t_cmd   *cmd;
    int     arg_count;

    if (!all || !all->card)
        return NULL;

    arg_count = args_counter(all);
    cmd = (t_cmd *)safe_malloc(all->collector, sizeof(t_cmd));
    if (!cmd)
        return NULL;

    cmd->args = NULL;
    cmd->redirects = NULL;
    cmd->next = NULL;
    cmd->prev = head_cmd;
    cmd->fd_in = 0;
    cmd->fd_out = 1;
    cmd->args_count = arg_count;

    cmd->args = (char **)safe_malloc(all->collector, sizeof(char *) * (arg_count + 1));
    if (!cmd->args)
        return NULL;

    for (int i = 0; i <= arg_count; i++)
        cmd->args[i] = NULL;

    cmd->redirect_count = redir_counter(all);
    if (cmd->redirect_count > 0)
    {
        cmd->redirects = (t_redirect *)safe_malloc(all->collector, 
                                                 sizeof(t_redirect) * cmd->redirect_count);
        if (!cmd->redirects)
            return NULL;
    }
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

void    set_cmd(t_all *all, t_cmd *current_cmd)
{
    t_card *current_card;
    int i;
    int redirect_index;

    if (!all || !all->card || !current_cmd)
    {
        return;
    }

    current_card = all->card;
    i = 0;
    redirect_index = 0;

    t_card *temp = current_card;
    while (temp && temp->type != PIPE)
    {
        temp = temp->next;
    }    
    while (current_card != NULL && current_card->type != PIPE)
    {
        if (current_card->type == WORD || current_card->type == -1)
        {
            if (i < current_cmd->args_count)
            {
                current_cmd->args[i] = current_card->value;
                i++;
            }
        }
        else if (current_card->type == R_APPEND || current_card->type == R_OUT || 
                 current_card->type == HEREDOC || current_card->type == R_IN)
        {
            if (redirect_index < current_cmd->redirect_count && current_card->next)
            {
                current_cmd->redirects[redirect_index].type = current_card->type;
                current_cmd->redirects[redirect_index].filename = current_card->next->value;
                redirect_index++;
                current_card = current_card->next;
            }
        }
        current_card = current_card->next;
    }
    
    current_cmd->args[i] = NULL;
}

void parser(t_all *all)
{
    t_cmd *head_cmd;
    t_cmd *current_cmd;
    t_card *current_card;

    head_cmd = NULL;
    current_card = all->card;

    while (current_card != NULL)
    {
        current_cmd = init_cmd(head_cmd, all);
        if (!current_cmd)
            return;

        put_node(&head_cmd, current_cmd);
        set_cmd(all, current_cmd);

        while (current_card && current_card->type != PIPE)
            current_card = current_card->next;
        
        if (current_card)
            current_card = current_card->next;
        
        all->card = current_card;
    }
    // Add this line to link commands to all structure
    all->cmd = head_cmd;
}
