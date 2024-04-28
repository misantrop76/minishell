/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:32:42 by mminet            #+#    #+#             */
/*   Updated: 2024/04/28 02:27:11 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	del(void *del)
{
	free(del);
}

void	handle_sigint(int sig)
{
	(void)sig;
	//printf("signal = %d\n", sig);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	t_list	*my_env;

	(void)av;
	if (ac != 1)
		return (1);
	signal(SIGINT, handle_sigint);
	my_env = NULL;
	get_env(&my_env, env);
	get_input(my_env);
	ft_lstclear(&my_env, del);
}
