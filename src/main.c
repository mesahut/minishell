/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:23:54 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/10 19:25:14 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*collector_dup(t_collector **collector, char *line)
{
	char	*str;
	int		i;

	i = 0;
	str = (char *)safe_malloc(collector, ft_strlen(line) + 1);
	while (line[i])
	{
		str[i] = line[i];
		i++;
	}
	str[i] = '\0';
	free(line);
	return (str);
}

void	print_card(t_card *card)
{
	t_card	*current;

	current = card;
	while (current)
	{
		printf("type: %d, Value: %s\n", current->type, current->value);
		current = current->next;
	}
}

void	reset_all(t_all *all)
{
	if (all->collector)
	{
		clean_malloc(all->collector);
		all->collector = NULL;
	}
	all->card = NULL;
	all->cmd = NULL;
}

void	free_env(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	main(int argc, char **argv, char **env_list)
{
	t_all	all;
	char	*line;
	char	*input;

	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_quit);
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
			free_env(all.env);
			reset_all(&all);
			printf("exit\n");
			all.exit_status = 0;
			return (0);
		}
		add_history(line);
		input = collector_dup(&all.collector, line);
		if (lexer(input, &all) == 0)
			continue ;
		if (expander(&all) == 1)
		{
			reset_all(&all);
			continue ;
		}
		parser(&all);
		exec(&all);
		reset_all(&all);
	}
	all.exit_status = 0;
	return (0);
}
