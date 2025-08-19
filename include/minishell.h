/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:47:34 by mayilmaz          #+#    #+#             */
/*   Updated: 2025/08/08 21:47:34 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define PIPE 0
# define R_APPEND 1
# define R_OUT 2
# define HEREDOC 3
# define R_IN 4
# define WORD 5

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_MISUSE 2
# define EXIT_CANNOT_EXECUTE 126
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_INVALID_EXIT 128
# define EXIT_CTRL_C 130


typedef struct s_collector
{
	void				*value;
	struct s_collector	*next;
}	t_collector;

typedef struct s_card
{
	int				type;
	int				here_flag;
	char			*value;
	struct s_card	*next;
}	t_card;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redirect
{
	int					type;
	char				*filename;
	char				*value;
	int					fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	char			**args;
	int				fd_in;
	int				fd_out;
	int				args_count;
	int				redirect_count;
	t_redirect		*redirects;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_all
{
	t_collector		*collector;
	t_card			*card;
	t_cmd			*cmd;
	t_env			*env;
	int				exit_status;
}	t_all;

extern int	g_signal;

void    set_cmd(t_card *cursor, t_all *all, t_cmd *cmd);
void	parser(t_all *all);
void	exec(t_all *all);
void	create_card(t_all *all, char *card);
int		expander(t_all *all);
int		lexer(char *line, t_all *all);
void	put_env(t_all *all, char **env_list);
void	*safe_malloc(t_all *all, int size);
void	clean_malloc(t_all *all);
char	*collector_dup(t_all *all, char *line);
int		exec_builtin(t_all *all, t_cmd *cmd);
int		is_builtin(char *cmd);
char	*expend_join(char const *s1, char const *s2, t_all *all);
int		flag_check(char *arg);
void	print_card(t_card *card);
void	reset_all(t_all *all, int status_type);
void	free_env(t_env *env_list);
void	free_split(char **split);

int		ft_pwd(t_all *all, t_cmd *cmd);
int		ft_cd(t_all *all, t_cmd *cmd);
int		ft_echo(t_all *all, t_cmd *cmd);
int		ft_export(t_all *all, t_cmd *cmd);
int		ft_unset(t_all *all, t_cmd *cmd);
int		ft_env(t_all *all, t_cmd *cmd);
void	print_sorted_env(t_all *all);
int		ft_exit(t_all *all, t_cmd *cmd);
t_env	*find_env_by_key(t_env *env_list, const char *key);
void	add_or_update_env(t_all *all, const char *key, const char *value);

int		check_for_expansion(t_all *all);
char	*found_dollar(char *line, int dollar_place, t_all *all);
char	is_char_quote(char value, char quote_type);

void	sig_int(int code);
void	sig_quit(int code);
void	sig_prompt(int sig);
void	signal_switch(int status);
// Exec utility functions
char	*path_find(char *cmd, t_all *all);
char	**list_to_envp(t_all *all);
void	exec_external_cmd(char *path, char **args, t_all *all);
char	*here_expand(char *str, t_all *all);
int		check_here_flag(t_card *card, char *eof);
void	handle_redirections(t_cmd *cmd, t_all *all);

// Exec pipeline functions
void	process_fork_cmd(t_cmd *cmd, t_all *all, int *prev_fd, int pipefd[2]);
int		process_builtin_cmd(t_cmd *cmd, t_all *all, int prev_fd);
void	exec_builtin_single(t_cmd *cmd, t_all *all, int prev_fd);
void	exec_child_process(t_cmd *cmd, t_all *all, int prev_fd, int pipefd[2]);
void	exec_parent_process(t_cmd *cmd, t_all *all, int *prev_fd, pid_t pid);
void	exec_pipeline(t_all *all);
// expand nomundan sonra eklenenler
void	del_quote(t_all *all);
void	put_title(t_all *all);
void	delim_node(t_all *all, t_card *node);
void	check_node(t_card *card, t_card *prev);
char	*ft_getenv(t_env *env, char *key);
// lexer normundan sonra eklenenler
int	is_space(char c);
int	special_case(char c, char next, int *place);
int	is_operator(char c, char next);
void	insert_node_at(t_all *all, t_card **pos, char *str);
void print_cmd(t_cmd *cmd);

int ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
int	ft_isalnum(int c);
int	ft_isdigit(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1, t_all *all);
char	*ft_strjoin(char const *s1, char const *s2, t_all *all);
char	*ft_substr(char const *s, unsigned int start, size_t len, t_all *all);
char	**ft_split(char const *s, char c, t_all *all);

#endif