/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:46:01 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/31 13:49:14 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>

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
