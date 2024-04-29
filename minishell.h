/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:30:44 by mminet            #+#    #+#             */
/*   Updated: 2024/04/29 12:40:00 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
#include <signal.h>

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
	char	*read_doc;
	char	**input;
	char	**out_a;
	char	**out;
	t_list	*cmd;
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

char		*get_next_line(int fd);
void		*get_env(t_list **lst, char **env);
char		*get_var(char *cmp, t_list *env);
void		get_input(t_list *my_env, char **env);
void		check_var(t_var *var, t_list *env, int *i, char *input);
t_token		*mk_token(char *type, char *value);
void		check_quote(char c, t_var *var, int *i);
int			parse_token(t_list *token, t_list *my_env, char **env);
int			is_operator(char *str);
void		del_token(void *to_del);
int			check_error(t_list *lst_token);
int			check_input(char *input, t_list *my_env, int status, char **env);

#endif