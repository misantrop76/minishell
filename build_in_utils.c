/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehay <ehay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:00:08 by mminet            #+#    #+#             */
/*   Updated: 2024/05/14 15:06:11 by ehay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_cd(char **cmd, t_list *env)
{
	char	buf[100];
	char	*str;

	if (cmd[1] && cmd[2])
	{
		ft_putstr_fd("cd : wrong number of arguments\n", 2);
		return (1);
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
		return (1);
	}
	free(str);
	return (0);
}

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
	if (code == 0)
		ft_putstr_fd("exit\n", 1);
	exit(code);
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
		ft_putstr_fd("exit: too many arguments\n", 2);
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
			ft_putstr_fd("exit: numeric argument required\n", 2);
			free_param(pipex, env, 2);
		}
	}
	ex = ft_atoi(pipex->cmd[1]);
	if (ex < 0)
		ex += 256;
	if (ex >= 0 && ex <= 255 && pipex->cmd)
		free_param(pipex, env, ex);
	else
		free_param(pipex, env, 1);
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
