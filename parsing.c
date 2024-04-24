/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:05:08 by mminet            #+#    #+#             */
/*   Updated: 2024/04/24 19:05:19 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token		mk_token(char *type, char *value)
{
	t_token	token;
	
	if (type != NULL)
		token.type = ft_strdup(type);
	else
		token.type = NULL;
	if (value != NULL)
		token.value = ft_strdup(value);
	else
		token.value = NULL;
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
	char	*str;
	int		j;
	char	*tmp;

	*i += 1;
	j = 0;
	str = malloc(sizeof(char) * (ft_strlen(input) + 1));
	while (input[*i] && ft_isalnum(input[*i]))
	{
		str[j] = input[*i];
		j++;
		*i += 1;
	}
	str[j] = '\0';
	tmp = ft_strdup(get_var(str, env));
	free(str);
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
		if ((input[*i] == '"' && var.quote_s == 0) || (input[*i] == 39 && var.quote == 0))
			check_quote(input[*i], &var, i);
		else if (input[*i] == '$' && var.quote_s == 0)
		{
			var.tmp = var.str;
			var.var_to_get = var_to_get(input, i, env);
			if (var.var_to_get)
			{
				var.str = ft_strjoin(var.str, var.var_to_get);
				free(var.var_to_get);
				free(var.tmp);
			}
		}
		else if (input[*i])
		{
			var.tmp = var.str;
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

static void	my_del(void *to_del)
{
	t_token *token;
	
	token = (t_token *)to_del;
	(void)token;
	//printf("%s\n", token->type);
	//printf("|%s|\n", token->value);
	//if (token->type)
	//	free(token->type);
	//if (token->type)
	//	free(token->value);
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
		if (!input[i])
			break;
		token = get_token(input, &i, env);
		printf("type = %s		value %s\n", token.type, token.value);
		ft_lstadd_back(&token_lst, ft_lstnew(&token));
	}
	t_list *tmp;
	t_token *tokens;
	tmp = token_lst;
	while (tmp)
	{
		tokens = (t_token *)tmp->content;
		printf("type = %s		value %s\n", tokens->type, tokens->value);
		tmp = tmp->next;
	}
	ft_lstclear(&token_lst, my_del);
}

void	get_input(t_list *env)
{
	char	*input;

	(void)env;
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
