/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 01:28:14 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/04 11:40:39 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	l;
	char	*sb;

	if (!s)
		return (NULL);
	i = 0;
	l = ft_strlen(s);
	if (start > l)
		len = 0;
	else if (l < len + start)
		len = l - start;
	sb = (char *)malloc ((len * sizeof(char)) + 1);
	if (!sb)
		return (0);
	while (len > i && s[i + start])
	{
		sb[i] = s[i + start];
		i++;
	}
	sb[i] = '\0';
	return (sb);
}
