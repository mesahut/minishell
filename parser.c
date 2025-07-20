#include "lexer.h"

void parser(t_all *all)
{
    t_card *current_card;

    current_card = all->card;
    while (current_card != NULL)
    {
        else if (current_card->type == PIPE)
        {
            
        }
        if (current_card->type == WORD)
        {
            // Handle pipe logic if necessary
        }
        else if (current_card->type == R_IN || current_card->type == R_OUT || current_card->type == R_APPEND || current_card->type == HEREDOC)
        {

        }
        current_card = current_card->next;
    }
}
