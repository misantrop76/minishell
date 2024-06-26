/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 23:42:30 by mminet            #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*mk_token(char *type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token) * 1);
	if (type != NULL)
		token->type = ft_strdup(type);
	else
		token->type = NULL;
	if (value != NULL)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
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
	if (var->quote || var->quote_s)
		var->is_quote = 1;
}

char	*var_name(char *input, int *i)
{
	char	*str;
	int		j;

	j = 0;
	str = malloc(sizeof(char) * (ft_strlen(input + *i) + 1));
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		str[j++] = input[*i];
		*i += 1;
	}
	str[j] = '\0';
	return (str);
}

char	*var_to_get(char *input, int *i, t_list *env, int status)
{
	char	*str;
	char	*var;

	*i += 1;
	if (!ft_isalpha(input[*i]) && input[*i] != '_' && input[*i] != '?')
	{
		*i += 1;
		return (ft_strdup(""));
	}
	str = var_name(input, i);
	var = NULL;
	if (ft_strlen(str) == 0 && input[*i] == '?')
	{
		var = ft_itoa(status);
		*i += 1;
	}
	else if (get_var(str, env) != NULL)
		var = ft_strdup(get_var(str, env));
	free(str);
	return (var);
}

void	check_var(t_var *var, t_list *env, int *i, char *input)
{
	char	tmp[2];

	tmp[1] = '\0';
	if (input[*i] == '$' && var->quote_s == 0 && input[*i + 1]
		&& input[*i + 1] != '"' && !is_ispace(input[*i + 1], 0))
	{
		var->tmp = var->str;
		var->var_to_get = var_to_get(input, i, env, var->status);
		if (var->var_to_get)
		{
			var->str = ft_strjoin(var->str, var->var_to_get);
			free(var->tmp);
			free(var->var_to_get);
		}
	}
	else if (input[*i])
	{
		var->tmp = var->str;
		tmp[0] = input[*i];
		var->str = ft_strjoin(var->str, tmp);
		*i += 1;
		free(var->tmp);
	}
}
