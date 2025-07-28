/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:33:51 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/22 09:39:06 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	ft_count(int c, int *i)
{
	int	b;

	b = 0;
	if (c < 0)
	{
		*i = 1;
		b++;
	}
	if (c == 0)
	{
		b++;
		return (b);
	}
	while (c != 0)
	{
		c = c / 10;
		b++;
	}
	return (b);
}

char	*ft_itoa(int n)
{
	int			b;
	int			i;
	long int	m;
	char		*s1;

	m = n;
	i = 0;
	b = ft_count (m, &i);
	s1 = (char *)malloc((b + 1) * sizeof(char));
	if (!s1)
		return (0);
	if (i == 1)
	{
		s1[0] = '-';
		m *= -1;
	}
	s1[b] = '\0';
	b--;
	while (i <= b)
	{
		s1[b] = (m % 10) + 48;
		m = m / 10;
		b--;
	}
	return (s1);
}
