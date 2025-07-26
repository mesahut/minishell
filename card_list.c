#include "lexer.h"

void    create_card(t_all *all, char *card)
{
    t_card  *new_card;
    t_card  *current;

    new_card = (t_card *) safe_malloc(all->collector ,sizeof(t_card));
    new_card->value = card;
    new_card->type = -1;
    new_card->here_flag = 0;
    new_card->next = NULL;
    if(all->card == NULL)
        all->card = new_card;
    else
    {
        current = all->card;
        while (current->next != NULL)
            current = current->next;
        current->next = new_card;
    }
}

void    create_collector(t_collector *head_card, void *address)
{
    t_collector  *new_card;
    t_collector  *current;

    new_card = (t_collector *) ft_calloc(sizeof(t_collector), 1);
    new_card->value = address;
    new_card->next = NULL;
    if(head_card == NULL)
        head_card = new_card;
    else
    {
        current = head_card;
        while (current->next != NULL)
            current = current->next;
        current->next = new_card;
    }
}

void    *safe_malloc(t_collector *gc_head, int size)
{
    char    *str;

    str = ft_calloc(size, 1);
    create_collector(gc_head, str);
    return (str);
}
