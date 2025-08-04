/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asezgin <asezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:52:26 by asezgin           #+#    #+#             */
/*   Updated: 2025/08/04 13:03:36 by asezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char    *collector_dup(t_collector **collector, char *line)
{
	char    *str;
	int     i;

	i = 0;
	str = (char *)safe_malloc(collector, ft_strlen(line) + 1);
	while(line[i])
	{
		str[i] = line[i];
		i++;
	}
	str[i] = '\0';
	free(line);
	return (str);
}

void    print_card(t_card *card)
{
	t_card *current = card;
	while (current)
	{
		printf("Card Type: %d, Value: %s\n", current->type, current->value);
		current = current->next;
	}
}

void reset_all(t_all *all)
{
	if (all->collector)
	{
		clean_malloc(all->collector);
		all->collector = NULL;
	}
	all->card = NULL;
	all->cmd = NULL;
}

void free_env(t_env *env_list)
{
	t_env *current = env_list;
	t_env *next;

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
	int i = 1;

	if (!split)
		return;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int main(int argc, char **argv, char **env_list)
{
	(void)argc;
	(void)argv;

	t_all   all;
	char    *line = NULL;
	char    *input;
	
	signal(SIGINT, sig_int);
	signal(SIGQUIT, sig_quit);

	all = (t_all){0};
   put_env(&all, env_list);
   while (1)
   {
	   line = readline("minishell>>");    
		if (line == NULL)
		{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(g_sig.exit_status);
		}
			add_history(line);
			input = collector_dup(&all.collector, line);
			if (lexer(input, &all) == 0)
				continue; // Eğer lexer başarısızsa, yeni bir satır oku
			expander(&all);
			parser(&all);
		exec(&all);
		reset_all(&all);
	}
	return 0;
}
