/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:00:08 by mminet            #+#    #+#             */
/*   Updated: 2024/05/16 18:50:51 by mminet           ###   ########.fr       */
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
	if (pipex->env)
		free_tab(pipex->env);
	ft_lstclear(pipex->token_lst, del_token);
	ft_lstclear(&pipex->pid_lst, simple_del);
	free_tab(pipex->cmd);
	if (code < 0)
		code += 256;
	if (code > 255)
		code = code % 256;
	if (code >= 0 && code <= 255)
		exit(code);
	exit(1);
}

int	make_exit(t_pipex *pipex, t_list **env)
{
	int	ex;
	int	i;

	i = 0;
	ft_putstr_fd("exit\n", 1);
	if (!pipex->cmd[1])
		free_param(pipex, env, 0);
	while (pipex->cmd[1][i] == ' ' || pipex->cmd[1][i] == '	')
		i++;
	if (pipex->cmd[1][i] == '+' || pipex->cmd[1][i] == '-')
		i++;
	while (pipex->cmd[1] && pipex->cmd[1][i])
	{
		if (!ft_isdigit(pipex->cmd[1][i++]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			free_param(pipex, env, 2);
		}
	}
	ex = ft_atoi(pipex->cmd[1]);
	if (!pipex->cmd[2])
		free_param(pipex, env, ex);
	ft_putstr_fd("exit: too many arguments\n", 2);
	return (1);
}

int	is_build_in(char *str)
{
	if (ft_strncmp(str, "cd", 2) == 0 && ft_strlen(str) == 2)
		return (1);
	else if (ft_strncmp(str, "pwd", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	else if (ft_strncmp(str, "echo", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	else if (ft_strncmp(str, "export", 6) == 0 && ft_strlen(str) == 6)
		return (1);
	else if (ft_strncmp(str, "unset", 5) == 0 && ft_strlen(str) == 5)
		return (1);
	else if (ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	else if (ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	return (0);
}
