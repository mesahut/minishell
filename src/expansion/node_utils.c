/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 00:28:13 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/11 15:03:14 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	check_node(t_card *card, t_card *prev)
{
	prev->next = card->next;
	card = prev;
	return ;
}

void	insert_node_at(t_all *all, t_card **pos, char *str)
{
	t_card	*tmp;
	t_card	*new_node;

	new_node = (t_card *)safe_malloc(all, sizeof(t_card));
	if (!new_node)
		return ;
	new_node->value = str;
	new_node->type = WORD;
	new_node->here_flag = 0;
	tmp = (*pos)->next;
	(*pos)->next = new_node;
	new_node->next = tmp;
}
