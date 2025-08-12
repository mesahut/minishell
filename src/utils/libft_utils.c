/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 18:07:59 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/12 18:09:47 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../include/minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *s1, t_all *all)
{
	int		i;
	int		size;
	char	*s2;

	i = 0;
	size = ft_strlen(s1);
	s2 = malloc((size + 1) * sizeof(char));
	if (!s2)
		reset_all(all, 12);
	while (s2 && size > i)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strjoin(char const *s1, char const *s2, t_all *all)
{
	int		i;
	int		len1;
	int		len2;
	char	*dst;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	dst = malloc(((len1 + len2) * sizeof(char)) + 1);
	if (!dst)
		reset_all(all, 12);
	while (i < len1)
	{
		dst[i] = s1[i];
		i++;
	}
	while (i < (len1 + len2))
	{
		dst[i] = s2[i - len1];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_substr(char const *s, unsigned int start, size_t len, t_all *all)
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
		reset_all(all, 12);
	while (len > i && s[i + start])
	{
		sb[i] = s[i + start];
		i++;
	}
	sb[i] = '\0';
	return (sb);
}
