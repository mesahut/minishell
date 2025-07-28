/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 22:24:18 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/26 15:13:08 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	ft_find(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*str;
	int		a;
	int		b;
	int		i;

	a = 0;
	i = 0;
	if (s1 == 0 || set == 0)
		return (NULL);
	b = ft_strlen(s1);
	while (s1[a] && ft_find(s1[a], (char *)set))
		a++;
	while (b > a && ft_find(s1[b - 1], (char *)set))
		b--;
	str = (char *)malloc(sizeof(char) * (b - a + 1));
	if (!str)
		return (NULL);
	while (a < b)
		str[i++] = s1[a++];
	str[i] = '\0';
	return (str);
}
