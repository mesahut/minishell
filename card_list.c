#include "lexer.h"

void    create_card(t_card **head_card, char *card)
{
    t_card  *new_card;
    t_card  *current;

    new_card = (t_card *) malloc(sizeof(t_card));
    new_card->value = card;
    new_card->type = -1;
    new_card->next = NULL;
    if(*head_card == NULL)
        *head_card = new_card;
    else
    {
        current = *head_card;
        while (current->next != NULL)
            current = current->next;
        current->next = new_card;
    }
}
