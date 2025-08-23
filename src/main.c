/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:23:54 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/24 00:14:36 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **env_list)
{
	t_all	all;
	char	*line;
	char	*input;

	signal_switch(1);
	(void)argc;
	(void)argv;
	line = NULL;
	all = (t_all){0};
	put_env(&all, env_list);
	while (1)
	{
		line = readline("minishell>>");
		if (line == NULL)
		{
			printf("exit\n");
			reset_all(&all, EXIT_SUCCESS);
		}
		if (g_signal == SIGINT)
		{
			all.exit_status = EXIT_CTRL_C;
			g_signal = 0;
		}
		add_history(line);
		input = collector_dup(&all, line);
		if (lexer(input, &all) == 1)
			continue ;
		if (expander(&all) == 1)
		{
			clean_malloc(&all);
			continue ;
		}
		parser(&all);
		exec(&all);
		clean_malloc(&all);
	}
	all.exit_status = 0;
	return (0);
}
