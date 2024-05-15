/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:00:08 by mminet            #+#    #+#             */
/*   Updated: 2024/05/14 21:24:21 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_pwd(char **cmd)
{
	char	buf[100];

	(void)cmd;
	if (getcwd(buf, 99))
	{
		ft_putstr_fd(buf, 1);
		ft_putstr_fd("\n", 1);
	}
	return (0);
}

void	free_param(t_pipex *pipex, t_list **env, int code)
{
	ft_lstclear(env, simple_del);
	ft_lstclear(pipex->token_lst, del_token);
	free_tab(pipex->cmd);
	if (code < 0)
		code += 256;
	if (code > 255)
		code = code % 256;
	ft_putstr_fd("exit\n", 1);
	if (code >= 0 && code <= 255)
		exit(code);
	exit(1);
}

void	make_exit(t_pipex *pipex, t_list **env)
{
	int	ex;
	int	i;

	i = 0;
	if (!pipex->cmd[1])
		free_param(pipex, env, 0);
	if (pipex->cmd[1] && pipex->cmd[2])
	{
		ft_putstr_fd("exit: trop d'arguments\n", 2);
		free_param(pipex, env, 1);
	}
	while (pipex->cmd[1][i] == ' ' || pipex->cmd[1][i] == '	')
		i++;
	if (pipex->cmd[1][i] == '+' || pipex->cmd[1][i] == '-')
		i++;
	while (pipex->cmd[1] && pipex->cmd[1][i])
	{
		if (!ft_isdigit(pipex->cmd[1][i++]))
		{
			ft_putstr_fd("exit: argument numérique nécessaire\n", 2);
			free_param(pipex, env, 2);
		}
	}
	ex = ft_atoi(pipex->cmd[1]);
	free_param(pipex, env, ex);
}

int	is_build_in(char *str)
{
	if (ft_strncmp(str, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(str, "pwd", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "echo", 4) == 0)
		return (1);
	else if (ft_strncmp(str, "export", 6) == 0)
		return (1);
	else if (ft_strncmp(str, "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(str, "env", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "exit", 4) == 0)
		return (1);
	return (0);
}
