/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:20:11 by mminet            #+#    #+#             */
/*   Updated: 2024/04/25 02:57:04 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	check_error(t_list *lst_token)
{
	t_token *token;
	t_list	*tmp;

	tmp = lst_token;
	while (tmp)
	{
		token = tmp->content;  
		if (ft_strncmp(token->type, "ERROR", 5) == 0)
			ft_putstr_fd("error\n", 1);
		tmp = tmp->next;
	}
}

void	parse_token(t_list *token_lst)
{
	t_list *tmp;
	t_token	*token;

	tmp = token_lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		printf("type = %s		value = %s\n", token->type, token->value);
		tmp = tmp->next;
	}
	check_error(token_lst);
}
