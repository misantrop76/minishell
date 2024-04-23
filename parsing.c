/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:05:08 by mminet            #+#    #+#             */
/*   Updated: 2024/04/23 18:33:24 by mminet           ###   ########.fr       */
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
	if (c == '"' && var->quote_s == 0)
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

char	*var_to_get(char *input, int *i)
{
	char	*str;
	int		j;
	int		len;

	*i += 1;
	j = 0;
	len = 0;
	while (input[*i + len] && ft_isalnum(input[*i + len]))
		len++;
	str = malloc(sizeof(char *) * (len + 1));
	str[len] = '\0';
	while (input[*i] && ft_isalnum(input[*i]))
	{
		str[j] = input[*i];
		j++;
		*i += 1;
	}
	return (str);
}

t_token	mk_word(char *input, int *i, t_list *env)
{
	t_var	var;
	char	c;
	
	var.quote = 0;
	var.quote_s = 0;
	var.str = ft_strdup("");
	while (input[*i] && (var.quote || var.quote_s || input[*i] != ' '))
	{	
		var.tmp = var.str;
		if ((input[*i] == '"' && var.quote_s == 0)|| input[*i] == 39)
			check_quote(input[*i], &var, i);
		else if (input[*i] == '$' && var.quote_s == 0)
		{
			var.var_to_get = var_to_get(input, *i);
			var.str = ft_strjoin(var.str, get_var(var.var_to_get, env));
			free(var.tmp);
		}
		else if (input[*i])
		{
			var.str = ft_strjoin(var.str, &input[*i]);
			i += 1;
			free(var.tmp);
		}
	}
	return (mk_token("WORD", var.str));
}

t_token		get_token(char *input, int *i, t_list *env)
{
	if (input[*i] == '|')
	{
		*i += 1;
		return (mk_token("PIPE", "|"));
	}
	else if (input[*i] == '<' && input[*i] != '<')
	{
		*i += 1;
		if (input[*i + 1] != '<')
			return(mk_token("STDIN", "<"));
		*i += 1;
		return (mk_token("READ", "<<"));
	}
	else if(input[*i] == '>')
	{
		*i += 1;
		if (input[*i + 1] != '>')
			return(mk_token("STDOUT", ">"));
		*i += 1;
		return (mk_token("STDOUT_A", ">>"));
	}
	else
		return (mk_word(input, i, env));
}

void	check_input(char *input, t_list *env)
{
	t_list	*token_lst;
	t_token	token;
	int		i;

	i = 0;
	token_lst = NULL;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '	')
			i++;
		if (input[i])
			token = get_token(input, &i, env);
		if (strncmp("ERROR", token.type, 5) == 0)
			exit(1);
		ft_lstadd_back(&token_lst, ft_lstnew(&token));
	}
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
		get_next_line(0);
	}
	free(input);
}
