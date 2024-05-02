/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:10:59 by mminet            #+#    #+#             */
/*   Updated: 2024/05/02 18:33:37 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_cd(char **cmd, t_list *env)
{
	char	buf[100];

	if (!cmd[1] || cmd[2])
	{
		ft_putstr_fd("cd : wrong number of arguments\n", 2);
		// exit(1);
	}
	if (chdir(cmd[1]))
	{
		strerror(errno);
		// exit(1);
	}
	if (getcwd(buf, 99))
		change_var(env, "PWD", getcwd(buf, 99));
	// exit(0);
}

void	make_pwd(char **cmd)
{
	char	buf[100];

	if (cmd[1])
	{
		ft_putstr_fd("pwd : wrong number of arguments\n", 2);
		// exit(1);
	}
	else if (getcwd(buf, 99))
	{
		ft_putstr_fd(buf, 1);
		ft_putstr_fd("\n", 1);
	}
}

void	make_export(char **cmd, t_list *env)
{
	int		i;
	char	c;

	if (!cmd[1] || cmd[2])
	{
		ft_putstr_fd("export : wrong number of arguments\n", 2);
		// exit(1);
	}
	i = 0;
	while (cmd[1][i] && cmd[1][i] != '=')
		i++;
	if (i == 0 || !cmd[1][i])
		exit(1);
	c = cmd[1][i];
	cmd[1][i] = '\0';
	if (get_var(cmd[1], env) == NULL)
	{
		cmd[1][i] = c;
		ft_lstadd_back(&env, ft_lstnew(ft_strdup(cmd[1])));
	}
	else
		change_var(env, cmd[1], cmd[1] + i + 1);
	// exit(0);
}

void	make_unset(char **cmd, t_list *env)
{
	t_list	*tmp;
	t_list	*to_del;
	char	*cmp;

	if (!cmd[1] || cmd[2])
	{
		ft_putstr_fd("unset : wrong number of arguments\n", 2);
		exit(1);
	}
	to_del = env;
	tmp = env;
	cmp = ft_strjoin(cmd[1], "=");
	while (to_del && ft_strncmp(cmp, to_del->content, ft_strlen(cmp)))
		to_del = to_del->next;
	free(cmp);
	if (to_del == NULL)
		return;
	if (env == to_del)
		env = env->next;
	else
	{
		while (tmp && tmp->next != to_del)
			tmp = tmp->next;
		tmp->next = tmp->next->next;
	}
	free(to_del->content);
	free(to_del);
	to_del = NULL;
	//exit(0);
}

void	make_echo(char **cmd)
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
		ft_putstr_fd(" ", 1);
		i++;
	}
	if (!flag)
		ft_putstr_fd("\n", 1);
	//exit(0);
}

void	make_env(char **cmd, t_list *env)
{
	t_list	*tmp;

	if (cmd[1])
	{
		ft_putstr_fd("env : wrong number of arguments\n", 2);
		exit(1);
	}
	tmp = env;
	while (tmp)
	{
		ft_putstr_fd(tmp->content, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	//exit(0);
}

void	is_build_in(char **cmd, t_list *env)
{
	(void)env;
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		make_cd(cmd, env);
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		make_pwd(cmd);
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		make_echo(cmd);
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		make_export(cmd, env);
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		make_unset(cmd, env);
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		make_env(cmd, env);
}