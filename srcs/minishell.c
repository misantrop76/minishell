/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:32:42 by mminet            #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	simple_del(void *del)
{
	free(del);
}

void	my_exit(t_list **my_env)
{
	ft_putstr_fd("exit\n", 1);
	ft_lstclear(my_env, simple_del);
	close(1);
	close(2);
	close(0);
}

int	main(int ac, char **av, char **env)
{
	t_list	*my_env;
	char	*tmp;
	int		i;

	(void)av;
	i = 3;
	while (i < 256)
		close(i++);
	if (ac != 1)
		return (1);
	my_env = NULL;
	get_env(&my_env, env);
	if (get_var("SHLVL", my_env))
	{
		tmp = ft_itoa(ft_atoi(get_var("SHLVL", my_env)) + 1);
		change_var(my_env, "SHLVL", tmp);
		free(tmp);
	}
	else
		ft_lstadd_back(&my_env, ft_lstnew(ft_strdup("SHLVL=1")));
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	get_input(&my_env);
	my_exit(&my_env);
}
