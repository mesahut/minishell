#include <stdlib.h>
#include <stdio.h>

#define PIPE 0
#define R_APPEND 1
#define R_OUT 2
#define HEREDOC 3
#define R_IN 4
#define U_WORD 5
#define S_WORD 6
#define D_WORD 7

typedef struct s_card 
{
    int     type;
    char    *value;
    struct s_card *next;
} t_card;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;


void    create_card(t_card **head_card, char *card,int type)
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

int main()
{
    t_card  *card;

    card = NULL;
    //TESTCASE 1    cat <<EOF | grep "foo" > result.txt
    create_card(&card, "cat", U_WORD);
    create_card(&card, "<<", HEREDOC);
    create_card(&card, "EOF", U_WORD);
    create_card(&card, "|", PIPE);
    create_card(&card, "grep", U_WORD);
    create_card(&card, "foo", D_WORD);
    create_card(&card, ">", R_OUT);
    create_card(&card, "result.txt", U_WORD);

    //TESTCASE 2    cat < input.txt | grep apple >> output.txt
    create_card(&card, "cat", U_WORD);
    create_card(&card, "<", R_IN);
    create_card(&card, "input.txt", U_WORD);
    create_card(&card, "|", PIPE);
    create_card(&card, "grep", U_WORD);
    create_card(&card, "apple", U_WORD);
    create_card(&card, ">>", R_APPEND);
    create_card(&card, "output.txt", U_WORD);

    //TESTCASE 3  (export A=ho)  e"c"ho "$A"sgeldiniz'ma$A'|cat
    create_card(&card, "echo", U_WORD);
}

