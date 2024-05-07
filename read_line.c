/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:20:23 by mminet            #+#    #+#             */
/*   Updated: 2024/05/07 13:41:07 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*print_prompt(t_list *env, int status)
{
	char	*str;
	char	*pwd;
	char	*tmp;
	int		i;

	pwd = get_var("PWD", env);
	if (!pwd)
		return (ft_strdup(">"));
	i = ft_strlen(pwd) - 1;
	while (i - 1 >= 0 && pwd[i - 1] != '/')
		i--;
	(void)status;
	if (status)
		str = ft_strdup("\001\e[00;31m\002> \001\e[00;1m\002 \001\e[36m\002");
	else
		str = ft_strdup("\001\e[00;32m\002> \001\e[00;1m\002 \001\e[36m\002");
	tmp = str;
	str = ft_strjoin(str, pwd + i);
	free(tmp);
	tmp = str;
	str = ft_strjoin(str, "\001\e[00;0m\002 ");
	free(tmp);
	return (str);
}

void	get_input(t_list **my_env)
{
	char	*input;
	int		status;
	char	*tmp;

	status = 0;
	tmp = print_prompt(*my_env, status);
	input = NULL;
	input = readline(tmp);
	free(tmp);
	while (input != NULL)
	{
		if (ft_strlen(input))
		{
			add_history(input);
			status = check_input(input, my_env, status);
		}
		free(input);
		tmp = print_prompt(*my_env, status);
		input = readline(tmp);
		free(tmp);
	}
	rl_clear_history();
}
