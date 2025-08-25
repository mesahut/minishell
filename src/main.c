/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:23:54 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/25 20:26:15 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cycle(t_all all, char *line, char *input)
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
	{
		clean_malloc(&all);
		return ;
	}
	if (expander(&all) == 1)
	{
		clean_malloc(&all);
		return ;
	}
	parser(&all);
	exec(&all);
	clean_malloc(&all);
}

void	env_not_found(t_all *all)
{
    char *pair[3];
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
        cwd = strdup(".");
    pair[0] = "PWD";
    pair[1] = cwd;
    pair[2] = NULL;
    create_env(pair, all);
    free(cwd);
    pair[0] = "SHLVL";
    pair[1] = "1";
    pair[2] = NULL;
    create_env(pair, all);
    pair[0] = "_";
    pair[1] = "minishell";
    pair[2] = NULL;
    create_env(pair, all);
}

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
	if(!env_list[0])
		env_not_found(&all);
	else
		put_env(&all, env_list);
	input = NULL;
	while (1)
		cycle(all, line, input);
	all.exit_status = 0;
	return (0);
}
