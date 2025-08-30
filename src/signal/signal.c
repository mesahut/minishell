/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/30 20:24:19 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>

int	g_signal = 0;

void	sig_cat_quit(int sig)
{
	g_signal = sig;
	printf("Quit (the core has been removed)\n");
}

void	sig_prompt(int sig)
{
	g_signal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sig_exc(int sig)
{
	rl_event_hook = NULL;
	g_signal = sig;
	printf("\n");
}
void	sig_heredoc(int sig)
{
	g_signal = sig;
	rl_done = 1;
}

int	do_nothing(void)
{
	return 0;
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
