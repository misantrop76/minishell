/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:05:08 by mminet            #+#    #+#             */
/*   Updated: 2024/04/25 02:28:30 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*mk_word(char *input, int *i, t_list *env)
{
	t_var	var;

	var.quote = 0;
	var.quote_s = 0;
	var.str = ft_strdup("");
	while (input[*i] && (var.quote || var.quote_s || input[*i] != ' '))
	{	
		if ((input[*i] == '"' && var.quote_s == 0) || (input[*i] == 39 && var.quote == 0))
			check_quote(input[*i], &var, i);
		else
			check_var(&var, env, i, input);
	}
	if (var.quote || var.quote_s)
		return(mk_token("ERROR", "ERROR"));
	var.token = mk_token("WORD", var.str);
	free(var.str);
	return (var.token);
}

t_token		*get_token(char *input, int *i, t_list *env)
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
			return(mk_token("STDIN", "<"));
		*i += 1;
		return (mk_token("READ", "<<"));
	}
	else if(input[*i] == '>')
	{
		*i += 1;
		if (input[*i] != '>')
			return(mk_token("STDOUT", ">"));
		*i += 1;
		return (mk_token("STDOUT_A", ">>"));
	}
	else
		return (mk_word(input, i, env));
}

static void	my_del(void *to_del)
{
	t_token *token;
	
	token = (t_token *)to_del;
	if (token->type)
		free(token->type);
	if (token->type)
		free(token->value);
	free(token);
}

void	check_input(char *input, t_list *env)
{
	t_list	*token_lst;
	t_token	*token;
	int		i;

	i = 0;
	token_lst = NULL;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '	')
			i++;
		if (!input[i])
			break;
		token = get_token(input, &i, env);
		if (ft_strlen(token->value) == 0)
			my_del(token);
		else
			ft_lstadd_back(&token_lst, ft_lstnew(token));
	}
	parse_token(token_lst);
	ft_lstclear(&token_lst, my_del);
}

void	get_input(t_list *env)
{
	char	*input;

	ft_putstr_fd(">", 1);
	input = get_next_line(0);
	while (input != NULL)
	{
		printf("input = %s\n", input);
		check_input(input, env);
		free(input);
		ft_putstr_fd(">", 1);
		input = get_next_line(0);
	}
}
