/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:00:08 by mminet            #+#    #+#             */
/*   Updated: 2024/05/06 15:27:59 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_cd(char **cmd, t_list *env)
{
	char	buf[100];
	char	*str;

	if (cmd[2] && cmd[3])
	{
		ft_putstr_fd("cd : wrong number of arguments\n", 2);
		exit(1);
	}
	if (!cmd[1] || ft_strncmp(cmd[1], "~", 2) == 0)
		str = ft_strdup(get_var("HOME", env));
	else
		str = ft_strdup(cmd[1]);
	if (!chdir(str))
	{
		if (getcwd(buf, 99))
			change_var(env, "PWD", getcwd(buf, 99));
	}
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	free(str);
	exit(0);
}

void	make_pwd(char **cmd)
{
	char	buf[100];

	if (cmd[1])
	{
		ft_putstr_fd("pwd : wrong number of arguments\n", 2);
		exit(1);
	}
	else if (getcwd(buf, 99))
	{
		ft_putstr_fd(buf, 1);
		ft_putstr_fd("\n", 1);
	}
	exit(0);
}

void	free_param(t_pipex *pipex, t_list **env, int code)
{
	ft_lstclear(env, simple_del);
	ft_lstclear(&pipex->token_lst, del_token);
	free_tab(pipex->cmd);
	exit (code);
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
		ft_putstr_fd("Exit: too many arguments\n", 1);
		free_param(pipex, env, 1);
	}
	while (pipex->cmd[1] && pipex->cmd[1][i])
	{
		if (!ft_isdigit(pipex->cmd[1][i++]))
		{
			ft_putstr_fd("Exit: numeric argument required\n", 1);
			free_param(pipex, env, 1);
		}
	}
	ex = ft_atoi(pipex->cmd[1]);
	if (ex >= 0 && ex <= 255 && pipex->cmd)
		free_param(pipex, env, ex);
	else
		free_param(pipex, env, 1);
}
