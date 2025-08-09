/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/08 09:02:13 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
# include <signal.h>
#include <unistd.h>

int g_signal = 0;

void sig_int(int sig)
{
    (void)sig;
    rl_replace_line("", 0); // satırı sil
    write(1, "\n", 1);      // yeni satıra geç
   	if(g_signal == 0)
		rl_on_new_line();       // readline'e yeni satıra geçmesini söyle
    rl_redisplay();         // prompt'u yeniden yaz
}

void sig_quit(int code)
{
    (void)code;
    g_signal = code;
}	
