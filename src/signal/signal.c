/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/31 17:17:36 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>

int	g_signal = 0;

int	do_nothing(void)
{
	return (0);
}

void	sig_pipe(int sig)
{
	g_signal = sig;
}

void	signal_switch(int status)
{
	g_signal = 0;
	if (status == 1)
	{
		rl_event_hook = NULL;
		signal(SIGPIPE, sig_pipe);
		signal(SIGINT, sig_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (status == 2)
	{
		rl_event_hook = NULL;
		signal(SIGPIPE, sig_pipe);
		signal(SIGINT, sig_exc);
		signal(SIGQUIT, sig_cat_quit);
	}
	else if (status == 3)
	{
		rl_event_hook = do_nothing;
		signal(SIGPIPE, sig_pipe);
		signal(SIGINT, sig_heredoc);
	}
}
