/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:20:40 by mminet            #+#    #+#             */
/*   Updated: 2024/04/25 17:27:19 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_operator(char *str)
{
	if (ft_strncmp(str, "PIPE", 4) == 0)
		return (1);
	else if (ft_strncmp(str, "STD", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "READ", 4) == 0)
		return (1);
	return (0);
}

int	check_error(t_list *lst_token)
{
	t_token	*token;
	t_token	*token_next;
	t_list	*tmp;

	tmp = lst_token;
	token = tmp->content;
	if (!tmp->next && ft_strncmp(token->type, "WORD", 4))
		return (parse_error(token->value));
	while (tmp && tmp->next)
	{
		token = tmp->content;
		token_next = tmp->next->content;
		if (ft_strncmp(token->type, "PIPE", 4) == 0 && tmp == lst_token)
			return (parse_error(token->value));
		if (ft_strncmp(token->type, "ERROR", 5) == 0)
			return (parse_error(token->value));
		if (is_operator(token->type) && (!tmp->next || ft_strncmp(token_next->type, "WORD", 4) != 0))
			return (parse_error(token->value));
		tmp = tmp->next;
	}
	return (0);
}
