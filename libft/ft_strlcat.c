/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:56:59 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/15 17:56:59 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srclen;
	size_t	dstlen;

	i = 0;
	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (size == 0)
		return (srclen);
	if (size <= dstlen)
		return (srclen + size);
	while (src[i] && (i + dstlen) < (size - 1))
	{
		dst[i + dstlen] = src[i];
		i++;
	}
	dst[i + dstlen] = 0;
	return (dstlen + srclen);
}
