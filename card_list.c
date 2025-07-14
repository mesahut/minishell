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
void    mid_card(t_card **current_node, char *str, int type)
{
    t_card* new_node;
    t_card* original_next;

    new_node = (t_card *)malloc(sizeof(t_card));
    if (new_node == NULL)
        return (NULL);
    new_node->data = str;
    new_node->type = type;
    new_node->next = NULL;
    original_next = current_node->next;
    current_node->next = new_node;
    new_node->next = original_next;
    return (new_node);
} 
