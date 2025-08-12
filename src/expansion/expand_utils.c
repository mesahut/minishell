/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 00:26:52 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/12 17:24:20 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getenv(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	flag_check(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char	*join_words(char **temp, char *str, int i, t_all *all)
{
	while (temp && temp[i])
	{
		str = ft_strjoin(str, " ", all);
		str = ft_strjoin(str, temp[i], all);
		i++;
	}
	return (str);
}

void	delim_node(t_all *all, t_card *node)
{
	char	*str;
	char	**temp;
	int		i;

	i = 1;
	str = NULL;
	temp = ft_split(node->value, ' ', all);
	node->value = collector_dup(all, temp[0]);
	if (temp[i] && flag_check(temp[i]))
	{
		insert_node_at(all, &node, ft_strdup(temp[i], all));
		node = node->next;
		i++;
	}
	else if (temp[i])
	{
		str = temp[i];
		i++;
	}
	str = join_words(temp, str, i, all);
	if (str)
		insert_node_at(all, &node, collector_dup(all, str));
	free(temp);
}

void	put_title(t_all *all)
{
	t_card	*current;

	current = all->card;
	while (current)
	{
		if (current->value[0] == '|')
			current->type = PIPE;
		else if (current->value[0] == '<' && current->value[1] == '<')
			current->type = HEREDOC;
		else if (current->value[0] == '<')
			current->type = R_IN;
		else if (current->value[0] == '>' && current->value[1] == '>')
			current->type = R_APPEND;
		else if (current->value[0] == '>')
			current->type = R_OUT;
		else
			current->type = WORD;
		current = current->next;
	}
}
