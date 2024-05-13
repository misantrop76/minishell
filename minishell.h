/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:30:44 by mminet            #+#    #+#             */
/*   Updated: 2024/05/13 21:50:06 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_token
{
	char	*type;
	char	*value;
}			t_token;

typedef struct s_pipex
{
	t_list	*tmp;
	t_list	**token_lst;
	t_list	*pid_lst;
	t_token	*token;
	int		status;
	int		*pid;
	int		old_stdout;
	int		old_stdin;
	char	**cmd;
}			t_pipex;

typedef struct s_var
{
	int		status;
	int		quote;
	int		quote_s;
	char	*str;
	char	*tmp;
	char	*var_to_get;
	t_token	*token;
}			t_var;

void		del_token(void *to_del);
int			ft_open(t_token *token_list, t_pipex *pipex);
char		*get_next_line(int fd);
int			make_build_in(char **cmd, t_list **env, t_pipex *pipex);
int			is_build_in(char *str);
void		*get_env(t_list **lst, char **env);
char		*get_var(char *cmp, t_list *env);
void		get_input(t_list **my_env);
void		check_var(t_var *var, t_list *env, int *i, char *input);
t_token		*mk_token(char *type, char *value);
void		check_quote(char c, t_var *var, int *i);
int			parse_token(t_list **token, t_list **my_env);
int			is_operator(char *str);
int			check_error(t_list *lst_token);
int			check_input(char *input, t_list **my_env, int status);
int			exec_line(t_list **token_lst, t_list **my_env);
char		**get_cmd(t_list *token_lst);
void		change_var(t_list *env, char *var_to_change, char *change);
int			make_cd(char **cmd, t_list *env);
int			make_pwd(char **cmd);
void		make_exit(t_pipex *pipex, t_list **env);
void		simple_del(void *del);
void		free_tab(char **cmd);
int			make_unset(char **cmd, t_list **env);
int			make_export(char **cmd, t_list **env);
char		**make_env_char(t_list *env);
int			is_pipe(t_list *token_lst);
void		unknown_command(t_pipex *pipex, char **path, t_list **env, char **my_env);
int			heredoc(char *limit);

#endif