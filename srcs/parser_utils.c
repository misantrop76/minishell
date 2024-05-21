/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:20:40 by mminet            #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_error(char *str)
{
	ft_putstr_fd("parse error near '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

int	is_operator(char *str)
{
	if (ft_strncmp(str, "STD", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "READ", 4) == 0)
		return (1);
	return (0);
}

static int	check_pipe(t_token *token, t_token *token_next)
{
	if (ft_strncmp(token->type, "ERROR", 5) == 0)
		return (1);
	if (ft_strncmp(token->type, "PIPE", 4) == 0 && ft_strncmp(token_next->type,
			"PIPE", 4) == 0)
		return (1);
	if (is_operator(token->type) && ft_strncmp(token_next->type, "WORD",
			4 != 0))
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
	if (ft_strncmp(token->type, "PIPE", 4) == 0 || ft_strncmp(token->type,
			"ERROR", 5) == 0)
		return (parse_error(token->value));
	while (tmp->next)
	{
		token_next = tmp->next->content;
		if (check_pipe(token, token_next))
			return (parse_error(token->value));
		tmp = tmp->next;
		token = tmp->content;
	}
	if (is_operator(token->type) || (ft_strncmp(token->type, "PIPE", 4) == 0))
		return (parse_error(token->value));
	return (0);
}
