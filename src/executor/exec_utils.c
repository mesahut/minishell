/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:44:33 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 21:10:59 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*here_expand(char *str, t_all *all)
{
	int		i;
	char	open_quote;
	char	*new_str;

	i = 0;
	open_quote = '\0';
	while (str[i])
	{
		open_quote = is_char_quote(str[i], open_quote);
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			new_str = found_dollar(str, i, all);
			free(str);
			str = ft_strdup(new_str, all);
			i = -1;
		}
		i++;
	}
	return (str);
}
