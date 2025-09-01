/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 00:29:38 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/31 20:49:57 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator(char c, char next)
{
	if ((c == '<' && next == '<') || (c == '>' && next == '>'))
		return (2);
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	special_case(char c, char next, int *place)
{
	int	len;

	len = is_operator(c, next);
	(*place) = (*place) + len;
	return (len);
}
