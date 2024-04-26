/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:20:23 by mminet            #+#    #+#             */
/*   Updated: 2024/04/26 14:42:00 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_input(t_list *env)
{
	char	*input;
	int		status;

	input = readline("->");
	status = 0;
	while (input != NULL)
	{
		if (ft_strlen(input))
		{
			add_history(input);
			status = check_input(input, env, status);
		}
		free(input);
		write(1, "hey", 3);
		input = readline("->");
	}
}

