/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:23:54 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/31 20:34:20 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>
#include <signal.h>

int	cycle(t_all *all, char *line, char *input)
{
	line = readline("minishell>>");
	if (line == NULL)
	{
		printf("exit\n");
		reset_all(all, EXIT_FAILURE);
	}
	if (g_signal == SIGINT)
	{
		all->exit_status = EXIT_CTRL_C;
		g_signal = 0;
	}
	if (line[0] != '\0')
		add_history(line);
	input = collector_dup(all, line);
	if (lexer(input, all) == 1)
		return (clean_malloc(all));
	if (expander(all) == 1)
	{
		cleanup_all_heredoc_fds(all);
		return (clean_malloc(all));
	}
	if (parser(all) == 1)
	{
		cleanup_all_heredoc_fds(all);
		return (clean_malloc(all));
	}
	if (exec(all) == 1)
	{
		cleanup_all_heredoc_fds(all);
		return (clean_malloc(all));
	}
	cleanup_all_heredoc_fds(all);
	clean_malloc(all);
	all->exit_status = 0;
	return (0);
}

void	env_not_found(t_all *all)
{
	char	*pair[3];
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup(".", all);
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

	line = NULL;
	all = (t_all){0};
	if (argc > 1)
	{
		printf("%s: No such file or directory\n", argv[1]);
		all.exit_status = 127;
		return (all.exit_status);
	}
	if (!env_list[0])
		env_not_found(&all);
	else
		put_env(&all, env_list);
	input = NULL;
	while (1)
	{
		signal_switch(1);
		cycle(&all, line, input);
		all.exit_flag = 0;
	}
	return (0);
}
