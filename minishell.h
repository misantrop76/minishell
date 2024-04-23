/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:30:44 by mminet            #+#    #+#             */
/*   Updated: 2024/04/23 18:25:47 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char		*get_next_line(int fd);
void		*get_env(t_list **lst, char **env);
char		*get_var(char *cmp, t_list *env);
void		*get_input(t_list *env);

typedef struct s_token
{
	char	*type;
	char	*value;
}			t_token;

typedef struct s_var
{
	int		quote;
	int		quote_s;
	char	*str;
	char	*tmp;
	char	*var_to_get;
}			t_var;

#endif