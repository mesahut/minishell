/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:56:44 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/24 12:32:35 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s1;
	int				i;

	i = 0;
	s1 = (unsigned char *)s;
	while (n > 0)
	{
		if (s1[i] == (unsigned char)c)
			return (s1 + i);
		i++;
		n--;
	}
	return (NULL);
}
