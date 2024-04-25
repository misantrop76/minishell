/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:20:11 by mminet            #+#    #+#             */
/*   Updated: 2024/04/25 17:56:28 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_error(char *str)
{
	ft_putstr_fd("parse error near '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

void	parse_redirection(t_list *token_lst)
{
	t_token *token;
	t_token *token_next;
	t_list	*tmp;
	t_list	*to_del;

	tmp = token_lst;
	while (tmp)
	{
		token = tmp->content;
		if (tmp->next)
			token_next = tmp->next->content;
		if (ft_strncmp(token->type, "STD", 3) == 0 || ft_strncmp(token->type, "READ", 3) == 0)
		{
			free(token->value);
			token->value = ft_strdup(token_next->value);
			to_del = tmp->next;
			tmp->next = tmp->next->next;
			ft_lstdelone(to_del, del_token);
		}
		tmp = tmp->next;
	}
}

void	init_pipex(t_pipex *pipex)
{
	pipex->input = NULL;
	pipex->read_doc = NULL;
	pipex->out = NULL;
	pipex->out_a = NULL;
}

void	fill_pipex(t_pipex *pipex, t_list *lst)
{
	t_list	*tmp;
	t_token	*token;

	init_pipex(pipex);
	while (tmp)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "STDIN", 5))
			pipex
	}
}

int	parse_token(t_list *token_lst)
{
	t_list	*tmp;
	t_token	*token;
	t_pipex pipex;
	int		status;


	status = check_error(token_lst);
	parse_redirection(token_lst);
	fill_pipex(&pipex, token_lst);
	tmp = token_lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		printf("type = %s		value = %s\n", token->type, token->value);
		tmp = tmp->next;
	}
	if (status)
		printf("%d\n", status);
	return (status);
}
