/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:56:54 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/26 16:24:21 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	a;
	char	*s;

	s = (char *)b;
	a = 0;
	while (a < len)
	{
		s[a] = (char)c;
		a++;
	}
	return (b);
}
