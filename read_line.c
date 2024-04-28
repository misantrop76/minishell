/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:20:23 by mminet            #+#    #+#             */
/*   Updated: 2024/04/27 23:29:37 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	my_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

char	*print_prompt(t_list *env, int status)
{
	char	*str;
	char	*pwd;
	char	*tmp;
	int		i;

	pwd = get_var("PWD", env);
	i = ft_strlen(pwd) - 1;
	if (!pwd)
		return ("");
	while (i - 1 >= 0 && pwd[i - 1] != '/')
		i--;
	if (status)
		str = ft_strdup("\033[31m-> \033[34m");
	else
		str = ft_strdup("\033[32m-> \033[34m");
	tmp = str;
	str = ft_strjoin(str, pwd + i);
	free(tmp);
	tmp = str;
	str = ft_strjoin(str," \033[0m");
	free(tmp);
	return (str);
}

void	get_input(t_list *env)
{
	char	*input;
	int		status;
	char	*tmp;

	status = 0;
	tmp = print_prompt(env, status);
	input = readline(tmp);
	free(tmp);
	while (input != NULL)
	{
		if (ft_strlen(input))
		{
			add_history(input);
			status = check_input(input, env, status);
		}
		free(input);
		tmp = print_prompt(env, status);
		input = readline(tmp);
		free(tmp);
	}
}

