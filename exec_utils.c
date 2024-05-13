/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 19:57:55 by mminet            #+#    #+#             */
/*   Updated: 2024/05/13 20:50:17 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void unknown_command(t_pipex *pipex, char **path, t_list **env, char **my_env)
{
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(pipex->cmd[0], 2);
	ft_putstr_fd("\n", 2);
	free_tab(path);
	free_tab(pipex->cmd);
	free_tab(my_env);
	ft_lstclear(env, simple_del);
	ft_lstclear(pipex->token_lst, del_token);
	rl_clear_history();
	exit(127);
}

char	**make_env_char(t_list *env)
{
	char	**new_env;
	t_list	*tmp;
	int		i;

	new_env = malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	i = 0;
	tmp = env;
	while (tmp)
	{
		new_env[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	is_pipe(t_list *token_lst)
{
	t_list	*tmp;
	t_token	*token;

	tmp = token_lst;
	while (tmp)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "PIPE", 4) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
