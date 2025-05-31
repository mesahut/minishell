#include "lexer.h"


int quote_checker(char *words)
{
    int i;
    char    open_quote;

    i = 0;
    open_quote = '\0';
    while (words[i])
    {
        if(open_quote == '\0' && (words[i] == 34 || words[i] == 39))
            open_quote = words[i];
        else if(open_quote == words[i])
            open_quote = '\0';
        i++;

    }
    return (open_quote = '\0'); // buraya bak
}

int pass_letter(char *line, int *place, char *status)
{
    int     i;
    char    quote;
    int     len;

    i = *place;
    len = 0;
    quote = *status;
    while(line[i] && (line[i] != ' ' || (quote == 34 || quote == 39)))
    {
        if(quote == '\0' && (line[i] == 34 || line[i] == 39))
            quote = line[i];
        else if(line[i] == quote)
            quote = '\0';
        i++;
        len++;
    }
    *place = i;
    *status = quote; 
    return(len);
}

void    split_line(char *line, t_card **card)
{
    int     i;
    int     start;
    int     end;
    char    quote;
    char    *str;

    i = 0;
    start = 0;
    end = 0;
    quote = '\0';
    while (line[i])
    {
        while ( line[i] && (line[i] == ' ' && quote == '\0'))
            i++;
        if(line[i] != ' ' && line[i])
        {
            start = i;
        }
        end = pass_letter(line, &i, &quote);
        str = ft_substr(line, start, end);
        create_card(card, str);
    }
}

void    lexer(char *line, t_card **card)
{
    quote_checker(line);
    split_line(line, card);
}