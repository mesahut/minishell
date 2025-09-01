/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/09/01 16:18:14 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <readline/readline.h>

int	g_signal = 0;

int	do_nothing(void)
{
	return (0);
}

void	signal_switch(int status)
{
	g_signal = 0;
	if (status == 1)
	{
		rl_event_hook = NULL;
		signal(SIGINT, sig_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (status == 2)
	{
		rl_event_hook = NULL;
		signal(SIGINT, sig_exc);
		signal(SIGQUIT, sig_cat_quit);
	}
	else if (status == 3)
	{
		rl_event_hook = do_nothing;
		signal(SIGINT, sig_heredoc);
	}
}
