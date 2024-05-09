/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:10:59 by mminet            #+#    #+#             */
/*   Updated: 2024/05/09 15:15:38 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_export(char **cmd, t_list **env)
{
	int		i;
	char	c;

	i = 0;
	while (cmd[1][i] && cmd[1][i] != '=')
		i++;
	if (i == 0 || !cmd[1][i])
		return (0);
	c = cmd[1][i];
	cmd[1][i] = '\0';
	if (get_var(cmd[1], *env) == NULL)
	{
		cmd[1][i] = c;
		ft_lstadd_back(env, ft_lstnew(ft_strdup(cmd[1])));
	}
	else
		change_var(*env, cmd[1], cmd[1] + i + 1);
	return (0);
}

int	make_env(char **cmd, t_list *env)
{
	t_list	*tmp;

	if (cmd[1])
	{
		ft_putstr_fd("env : wrong number of arguments\n", 2);
		return (1);
	}
	tmp = env;
	while (tmp)
	{
		if (cmd[1] && ft_strncmp("export", cmd[1], 6) == 0)
			ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->content, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	return (0);
}

int	make_unset(char **cmd, t_list **env)
{
	t_list	*tmp;
	t_list	*to_del;
	char	*cmp;

	to_del = *env;
	tmp = *env;
	if (!cmd[1] || get_var(cmd[1], *env) == NULL)
		return (0);
	cmp = ft_strjoin(cmd[1], "=");
	while (to_del && ft_strncmp(cmp, to_del->content, ft_strlen(cmp)))
		to_del = to_del->next;
	free(cmp);
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
		ft_putstr_fd(" ", 1);
		i++;
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
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		return (make_export(cmd, env));
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		return (make_unset(cmd, env));
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		return (make_env(cmd, *env));
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		make_exit(pipex, env);
	return (0);
}
