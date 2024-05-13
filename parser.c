/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:20:11 by mminet            #+#    #+#             */
/*   Updated: 2024/05/13 17:18:40 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_len_cmd(t_list *token_lst)
{
	int		len;
	t_list	*tmp;
	t_token	*token;

	len = 0;
	tmp = token_lst;
	token = tmp->content;
	while (tmp && ft_strncmp(token->type, "PIPE", 4) != 0)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "WORD", 4) == 0)
			len++;
		tmp = tmp->next;
	}
	return (len);
}

char	**get_cmd(t_list *token_lst)
{
	char	**cmd;
	t_list	*tmp;
	t_token	*token;
	int		len;

	len = get_len_cmd(token_lst);
	if (!len)
		return (NULL);
	cmd = malloc(sizeof(char *) * (len + 1));
	tmp = token_lst;
	token = tmp->content;
	len = 0;
	while (tmp && ft_strncmp(token->type, "PIPE", 4) != 0)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "WORD", 4) == 0)
		{
			cmd[len] = ft_strdup(token->value);
			len++;
		}
		tmp = tmp->next;
	}
	cmd[len] = NULL;
	return (cmd);
}

void	parse_redirection(t_list *token_lst)
{
	t_token	*token;
	t_token	*token_next;
	t_list	*tmp;
	t_list	*to_del;

	tmp = token_lst;
	while (tmp)
	{
		token = tmp->content;
		if (tmp->next)
			token_next = tmp->next->content;
		if (ft_strncmp(token->type, "STD", 3) == 0 || ft_strncmp(token->type,
				"READ", 3) == 0)
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

void	free_tab(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
		free(cmd[i++]);
	free(cmd);
}

int	parse_token(t_list **token_lst, t_list **my_env)
{
	int	status;

	if (!token_lst)
		return (0);
	status = check_error(*token_lst);
	if (status)
		return (status);
	parse_redirection(*token_lst);
	status = exec_line(token_lst, my_env);
	return (status);
}
