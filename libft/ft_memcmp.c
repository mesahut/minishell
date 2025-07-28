/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:56:47 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/24 12:32:45 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*dst1;
	unsigned char	*dst2;

	i = 0;
	dst1 = (unsigned char *)s1;
	dst2 = (unsigned char *)s2;
	while (n > i)
	{
		if (dst1[i] == dst2[i])
			i++;
		else
			return (dst1[i] - dst2[i]);
	}
	return (0);
}
