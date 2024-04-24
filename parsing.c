/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:05:08 by mminet            #+#    #+#             */
/*   Updated: 2024/04/24 03:34:01 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token		mk_token(char *type, char *value)
{
	t_token	token;

	token.type = ft_strdup(type);
	token.value = ft_strdup(value);
	return (token);
}

void	check_quote(char c, t_var *var, int *i)
{
	if (c == '"')
	{
		if (var->quote == 1)
			var->quote = 0;
		else
			var->quote = 1;
		*i += 1;
	}
	if (c == 39)
	{
		if (var->quote_s == 1)
			var->quote_s = 0;
		else
			var->quote_s = 1;
		*i += 1;
	}
}

char	*var_to_get(char *input, int *i, t_list *env)
{
	char	str[ft_strlen(input)];
	int		j;
	char	*tmp;

	*i += 1;
	j = 0;
	while (input[*i] && ft_isalnum(input[*i]))
	{
		str[j] = input[*i];
		j++;
		*i += 1;
	}
	str[j] = '\0';
	tmp = ft_strdup(get_var(str, env));
	return (tmp);
}

t_token	mk_word(char *input, int *i, t_list *env)
{
	t_var	var;
	char	tmp[2];

	tmp[1] = '\0';	
	var.quote = 0;
	var.quote_s = 0;
	var.str = ft_strdup("");
	while (input[*i] && (var.quote || var.quote_s || input[*i] != ' '))
	{	
		var.tmp = var.str;
		if ((input[*i] == '"' && var.quote_s == 0) || (input[*i] == 39 && var.quote == 0))
			check_quote(input[*i], &var, i);
		else if (input[*i] == '$' && var.quote_s == 0)
		{
			var.var_to_get = var_to_get(input, i, env);
			var.str = ft_strjoin(var.str, var.var_to_get);
			free(var.var_to_get);
			free(var.tmp);
		}
		else if (input[*i])
		{
			tmp[0] = input[*i];
			var.str = ft_strjoin(var.str, tmp);
			*i += 1;
			free(var.tmp);
		}
	}
	t_token test = mk_token("WORD", var.str);
	free(var.str);
	return (test);
}

t_token		get_token(char *input, int *i, t_list *env)
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

static void	del(void *to_del)
{
	t_token *token;
	
	token = (t_token *)to_del;
	free(token->type);
	free(token->value);
}

void	check_input(char *input, t_list *env)
{
	t_list	*token_lst;
	t_token	token;
	int		i;

	i = 0;
	token_lst = NULL;
	printf("input = %s\n", input);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '	')
			i++;
		if (!input[i])
			break;
		token = get_token(input, &i, env);
		printf("token = %s			value = %s\n", token.type, token.value);
		ft_lstadd_back(&token_lst, ft_lstnew(&token));
	}
	ft_lstclear(&token_lst, del);
}

void	get_input(t_list *env)
{
	char	*input;

	(void)env;
	ft_putstr_fd(">", 1);
	input = get_next_line(0);
	while (input)
	{
		check_input(input, env);
		free(input);
		input = NULL;
		ft_putstr_fd(">", 1);
		input = get_next_line(0);
	}
}
