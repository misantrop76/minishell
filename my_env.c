/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:59:45 by mminet            #+#    #+#             */
/*   Updated: 2024/05/03 15:25:36 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_var(t_list *env, char *var_to_change, char *change)
{
	t_list	*tmp;
	char	*cmp;

	cmp = ft_strjoin(var_to_change, "=");
	tmp = env;
	while (tmp && ft_strncmp(cmp, tmp->content, ft_strlen(cmp)))
		tmp = tmp->next;
	if (tmp)
	{
		free(tmp->content);
		tmp->content = ft_strjoin(cmp, change);
	}
	else
		ft_lstadd_back(&env, ft_lstnew(ft_strjoin(cmp, change)));
	free(cmp);
}

void	*get_env(t_list **lst, char **env)
{
	int		i;
	char	*content;

	i = 0;
	while (env[i] != NULL)
	{
		content = ft_strdup(env[i]);
		ft_lstadd_back(lst, ft_lstnew(content));
		i++;
	}
	return (NULL);
}

char	*get_var(char *cmp, t_list *env)
{
	t_list	*tmp;
	int		i;
	char	*cpy;

	tmp = env;
	cmp = ft_strjoin(cmp, "=");
	while (tmp && ft_strncmp(cmp, (char *)tmp->content, ft_strlen(cmp)) != 0)
		tmp = tmp->next;
	free(cmp);
	if (!tmp)
		return (NULL);
	cpy = (char *)tmp->content;
	i = 0;
	while (cpy[i] && cpy[i] != '=')
		i++;
	if (!cpy)
		return (NULL);
	return (cpy + i + 1);
}
