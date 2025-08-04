/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:42 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/04 10:00:56 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
# include <signal.h>
#include <unistd.h>

t_sig g_sig = {0}; 

void	*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

void	sig_int(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		ft_putstr_fd("\n", STDERR);
		ft_putstr_fd("minishell>>", STDERR);
		g_sig.exit_status = 1;
	}
	else
	{
		ft_putstr_fd("\n", STDERR);
		g_sig.exit_status = 130;
	}
	g_sig.sigint = 1;
}

void	sig_quit(int code)
{
	(void)code;
	printf("Quit: %d\n", code);
}

void	sig_init(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}