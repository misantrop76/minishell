/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:59:45 by mminet            #+#    #+#             */
/*   Updated: 2024/04/23 13:00:26 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (tmp && ft_strncmp(cmp, (char *)tmp->content, ft_strlen(cmp)))
		tmp = tmp->next;
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
