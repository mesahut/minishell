/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/10 10:27:09 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <unistd.h>

int	g_signal = 0;

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	if (g_signal == 0)
		rl_on_new_line();
	rl_redisplay();
}

void	sig_quit(int code)
{
	(void)code;
	g_signal = code;
}
