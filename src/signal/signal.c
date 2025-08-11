/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/11 12:20:22 by asezgin          ###   ########.fr       */
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

void	signal_switch(int status)
{
	g_signal = 0;
	if (status == 1)
		signal(SIGINT, sig_prompt);
	else if (status == 2)
		signal(SIGINT, sig_exc);
	else if (status == 3)
		signal(SIGINT, sig_heredoc);
}

void	sig_quit(int code)
{
	(void)code;
	g_signal = code;
}
