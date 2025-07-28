/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:57:07 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/28 18:21:29 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				i;
	unsigned char	*a1;
	unsigned char	*a2;

	i = 0;
	a1 = (unsigned char *)s1;
	a2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while ((a1[i] == a2[i]) && (n - 1 > 0) && (a1[i] != '\0'))
	{
		i++;
		n--;
	}
	return (a1[i] - a2[i]);
}
