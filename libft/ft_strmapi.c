/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:26:02 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/29 02:06:18 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		len;
	char	*s1;

	if (!s || !f)
		return (0);
	i = 0;
	len = ft_strlen(s);
	s1 = (char *)malloc(sizeof(char) * (len + 1));
	if (!s1)
		return (0);
	while (0 < len)
	{
		s1[i] = f(i, s[i]);
		len--;
		i++;
	}
	s1[i] = '\0';
	return (s1);
}
