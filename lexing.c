/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:05:08 by mminet            #+#    #+#             */
/*   Updated: 2024/05/15 03:16:14 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_ispace(char c, int op)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	if (op)
		if (c == '|' || c == '>' || c == '<')
			return (1);
	return (0);
}

t_token	*mk_word(char *input, int *i, t_list *env, int status)
{
	t_var	var;

	var.quote = 0;
	var.quote_s = 0;
	var.str = ft_strdup("");
	var.status = status;
	while (input[*i] && (var.quote || var.quote_s || is_ispace(input[*i],
				1) == 0))
	{
		if ((input[*i] == '"' && var.quote_s == 0) || (input[*i] == 39
				&& var.quote == 0))
			check_quote(input[*i], &var, i);
		else
			check_var(&var, env, i, input);
	}
	if (var.quote || var.quote_s)
		return (mk_token("ERROR", "\""));
	var.token = mk_token("WORD", var.str);
	free(var.str);
	return (var.token);
}

t_token	*get_token(char *input, int *i, t_list *env, int status)
{
	if (input[*i] == '|')
	{
		*i += 1;
		return (mk_token("PIPE", "|"));
	}
	else if (input[*i] == '<')
	{
		*i += 1;
		if (input[*i] != '<')
			return (mk_token("STDIN", "<"));
		*i += 1;
		return (mk_token("READ", "<<"));
	}
	else if (input[*i] == '>')
	{
		*i += 1;
		if (input[*i] != '>')
			return (mk_token("STDOUT", ">"));
		*i += 1;
		return (mk_token("STDOUT_A", ">>"));
	}
	else
		return (mk_word(input, i, env, status));
}

void	del_token(void *to_del)
{
	t_token	*token;

	token = (t_token *)to_del;
	if (token->type)
		free(token->type);
	if (token->type)
		free(token->value);
	free(token);
}

int	check_input(char *input, t_list **my_env, int status)
{
	t_list	*token_lst;
	t_token	*token;
	int		i;

	i = 0;
	token_lst = NULL;
	while (input[i])
	{
		while (is_ispace(input[i], 0))
			i++;
		if (!input[i])
			break ;
		token = get_token(input, &i, *my_env, status);
		if (ft_strlen(token->value) == 0)
			del_token(token);
		else
			ft_lstadd_back(&token_lst, ft_lstnew(token));
	}
	if (!token_lst)
	{
		// ft_putstr_fd("command not found: ''\n", 2);
		ft_lstclear(&token_lst, del_token);
		return (0);
	}
	status = parse_token(&token_lst, my_env);
	ft_lstclear(&token_lst, del_token);
	return (status);
}
