#include "../../include/minishell.h"
#include <stdlib.h>


char	*expend_join(char const *s1, char const *s2)
{
	int		i;
	int		len1;
	int		len2;
	char	*dst;
	i = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	dst = malloc(((len1 + len2) * sizeof(char)) + 1);
	if (!dst)
    return (0);
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
