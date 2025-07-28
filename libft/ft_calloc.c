/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:56:25 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/15 17:56:25 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	size_t	byte;
	char	*x;

	i = 0;
	byte = nmemb * size;
	if (size && (byte / size != nmemb))
		return (0);
	x = (char *)malloc(byte);
	if (!x)
		return (0);
	while (byte > 0)
	{
		x[i] = 0;
		i++;
		byte--;
	}
	return ((void *)x);
}
