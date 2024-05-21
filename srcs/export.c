/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:10:01 by mminet            #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	export_var(char *str, t_list **env)
{
	int		i;
	char	c;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	c = str[i];
	str[i] = '\0';
	if (get_var(str, *env) == NULL)
	{
		str[i] = c;
		ft_lstadd_back(env, ft_lstnew(ft_strdup(str)));
	}
	else
		change_var(*env, str, str + i + 1);
	return (0);
}

int	make_export(char **cmd, t_list **env)
{
	int	i;
	int	j;
	int	status;

	i = 1;
	status = 0;
	while (cmd[i])
	{
		j = 0;
		while (ft_isalnum(cmd[i][j]) || cmd[i][j] == '_')
			j++;
		if ((ft_isalpha(cmd[i][0]) == 0 && cmd[i][0] != '_') || cmd[i][0] == '='
			|| (cmd[i][j] && cmd[i][j] != '='))
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			status = 1;
		}
		else if (cmd[i][j] == '=')
			export_var(cmd[i], env);
		i++;
	}
	return (status);
}

void	unset_var(char *str, t_list **env)
{
	t_list	*to_del;
	t_list	*tmp;
	char	*cmp;

	cmp = ft_strjoin(str, "=");
	to_del = *env;
	while (to_del && ft_strncmp(cmp, to_del->content, ft_strlen(cmp)))
		to_del = to_del->next;
	free(cmp);
	if (to_del == NULL)
		return ;
	tmp = *env;
	if (*env == to_del)
		*env = (*env)->next;
	else
	{
		while (tmp && tmp->next != to_del)
			tmp = tmp->next;
		tmp->next = tmp->next->next;
	}
	free(to_del->content);
	free(to_del);
}

int	make_unset(char **cmd, t_list **env)
{
	int	i;
	int	j;
	int	status;

	i = 1;
	if (!cmd[1])
		return (0);
	status = 0;
	while (cmd[i])
	{
		j = 0;
		while (ft_isalnum(cmd[i][j]) || cmd[i][j] == '_')
			j++;
		if ((ft_isalpha(cmd[i][0]) == 0 && cmd[i][0] != '_') || cmd[i][j])
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			status = 1;
		}
		else
			unset_var(cmd[i], env);
		i++;
	}
	return (status);
}
