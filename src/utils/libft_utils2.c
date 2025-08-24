/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:00:00 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/24 14:17:34 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i] != '\0')
		i++;
	return (i);
}

int	is_redir(int type)
{
	if (type == R_APPEND || type == R_OUT || type == HEREDOC
		|| type == R_IN || type == R_ERR_OUT || type == R_ERR_APPEND)
	{
		return (1);
	}
	return (0);
}
