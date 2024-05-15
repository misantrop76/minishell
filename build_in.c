/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:10:59 by mminet            #+#    #+#             */
/*   Updated: 2024/05/15 03:15:48 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_new_pwd(char *str, t_list *env)
{
	char	*tmp;

	tmp = NULL;
	if (!str || str[0] == '~')
		tmp = get_var("HOME", env);
	if (str && tmp)
		tmp = ft_strjoin(tmp, str + 1);
	else if (str)
		tmp = ft_strdup(str);
	else if (tmp)
		tmp = ft_strdup(tmp);
	return (tmp);
}

int	make_cd(char **cmd, t_list *env)
{
	char	buf[100];
	char	*str;

	if (cmd[1] && cmd[2])
		return (write(2, "cd : too many arguments\n", 23) - 22);
	str = get_new_pwd(cmd[1], env);
	if (!str)
		return (write(2, "cd: HOME not set\n", 30) - 29);
	if (!chdir(str))
	{
		if (getcwd(buf, 99))
			change_var(env, "PWD", getcwd(buf, 99));
		free(str);
	}
	else
	{
		ft_putstr_fd("cd : ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(str);
		return (1);
	}
	return (0);
}

int	make_env(char **cmd, t_list *env)
{
	t_list	*tmp;

	if (cmd[1])
	{
		ft_putstr_fd("env : too many arguments\n", 2);
		return (1);
	}
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp("export", cmd[0], 6) == 0)
			ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->content, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	return (0);
}

int	make_echo(char **cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (cmd[1] && ft_strncmp(cmd[1], "-n", 2) == 0)
	{
		i++;
		flag = 1;
	}
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], 1);
		i++;
		if (cmd[i])
			ft_putstr_fd(" ", 1);
	}
	if (!flag)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	make_build_in(char **cmd, t_list **env, t_pipex *pipex)
{
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		return (make_cd(cmd, *env));
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		return (make_pwd(cmd));
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		return (make_echo(cmd));
	else if (ft_strncmp(cmd[0], "env", 3) == 0 || (ft_strncmp(cmd[0], "export",
				6) == 0 && !cmd[1]))
		return (make_env(cmd, *env));
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		return (make_export(cmd, env));
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		return (make_unset(cmd, env));
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		make_exit(pipex, env);
	return (0);
}
