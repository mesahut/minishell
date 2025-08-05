#include "../../include/minishell.h"

int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

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
    if (open_quote != '\0')
    {
        fprintf(stderr, "syntax error: unclosed quote\n");
        return (0);
    }
    return (1);
}

int	is_operator(char c, char next)
{
	if ((c == '<' && next == '<') || (c == '>' && next == '>'))
        return (2);
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	pass_letter(char *line, int *place, char *status)
{
	int		i;
	char	quote;
	int		len;

	i = *place;
	len = 0;
	quote = *status;
	while (line[i] && (quote != '\0' || !is_space(line[i])))
	{
		if (quote == '\0')
		{
			if (is_operator(line[i], line[i + 1]))
				break;
			if (line[i] == '\'' || line[i] == '\"')
				quote = line[i];
		}
		else if (line[i] == quote)
			quote = '\0';
		i++;
		len++;
	}
	*place = i;
	*status = quote;
	return (len);
}
int special_case(char c, char next, int *place)
{
    int len;

    len = is_operator(c, next);
    (*place) = (*place) + len;
	return (len);
}

void    split_line(char *line, t_all *all)
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
        while ( line[i] && (is_space(line[i]) && quote == '\0'))
            i++;
        if(line[i] && !is_space(line[i]))
            start = i;
        if (line[i] && is_operator(line[i], line[i + 1]))
            end = special_case(line[i], line[i + 1], &i);
        else
            end = pass_letter(line, &i, &quote);
        str = ft_substr(line, start, end);
        if(str)
        {
            if(str[0] != '\0')
                create_card(all, str);
            else
                free(str);
        }
    }
}

int    line_checker(char *line)
{
    int i;

    if (!line || line[0] == '\0')
        return (0); // exit yerine yeni readlinea geçmeli
    i = 0;
    while (line[i])
    {
        if(!is_space(line[i]))
            return (1);
        i++;
    }
    return (0); // exit yerine yeni readlinea geçmeli
}

int    lexer(char *line, t_all *all)
{
    if (line_checker(line) == 0)
        return (0);
    if (quote_checker(line) == 0)
        return (0);
    split_line(line, all);
    return (1);
}