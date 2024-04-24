/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:32:42 by mminet            #+#    #+#             */
/*   Updated: 2024/04/24 02:20:24 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	del(void *del)
{
	free(del);
}

int	main(int ac, char **av, char **env)
{
	t_list	*my_env;

	(void)av;
	if (ac != 1)
		return (1);
	my_env = NULL;
	get_env(&my_env, env);
	get_input(my_env);
	ft_lstclear(&my_env, del);
}
