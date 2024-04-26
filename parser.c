/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:20:11 by mminet            #+#    #+#             */
/*   Updated: 2024/04/26 16:24:57 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	tmp = lst;
	while (tmp)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "STDIN", 5))
			printf("hey\n");
		tmp = tmp->content;
	}
}

void print_token(t_list *token_lst)
{
	t_list *tmp;
	t_token *token;

	tmp = token_lst;
	while (tmp)
	{
		token = (t_token *)tmp->content;
		printf("type = %s		value = %s\n", token->type, token->value);
		tmp = tmp->next;
	}
}

int	parse_token(t_list *token_lst)
{
	int		status;

	if (!token_lst)
		return (0);
	status = check_error(token_lst);
	parse_redirection(token_lst);
	if (!status)
		print_token(token_lst);
	return (status);
}
