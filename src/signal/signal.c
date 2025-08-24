/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/24 15:45:52 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>

int	g_signal = 0;

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
	g_signal = sig;
	printf("\n");
}

void	sig_heredoc(int sig)
{
	g_signal = sig;
	rl_replace_line("", 0);
	close(0);
}

void	siq_quit(int sig)
{
	g_signal = sig;
	printf("Quit (core dumped)\n");
	rl_replace_line("", 0);
	//set_exit_status(128 + sig);
}

void	signal_switch(int status)
{
	g_signal = 0;
	if (status == 1)
	{
		signal(SIGQUIT, siq_quit);
		signal(SIGINT, sig_prompt);
	}
	else if (status == 2)
	{
		signal(SIGQUIT, siq_quit);
		signal(SIGINT, sig_exc);
	}
	else if (status == 3)
		signal(SIGINT, sig_heredoc);
}

