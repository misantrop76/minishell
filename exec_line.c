/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/05/10 15:17:31 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**make_env_char(t_list *env)
{
	char	**new_env;
	t_list	*tmp;
	int		i;
	char	*shlvl;

	new_env = malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	i = 0;
	tmp = env;
	if (get_var("SHLVL", env))
		shlvl = ft_itoa(atoi(get_var("SHLVL", env)) + 1);
	while (tmp)
	{
		if (ft_strncmp("SHLVL=", tmp->content, 6) == 0)
		{
			new_env[i] = ft_strjoin("SHLVL=", shlvl);
			free(shlvl);
		}
		else
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

void	exec_cmd(char **cmd, t_list **env, t_pipex *pipex)
{
	char	*newpath;
	char	**path;
	char	**my_env;
	char	*tmp;
	int		i;

	newpath = NULL;
	i = 0;
	my_env = make_env_char(*env);
	if (is_build_in(cmd[0]))
		exit(make_build_in(cmd, env, pipex));
	if (get_var("PATH", *env))
		path = ft_split(get_var("PATH", *env), ':');
	else
		path = ft_split("/bin:/sbin", ':');
	execve(cmd[0], cmd, my_env);
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		newpath = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		execve(newpath, cmd, my_env);
		free(newpath);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd("\n", 2);
	free_tab(path);
	free_tab(my_env);
	exit(127);
}

void	check_cmd(t_pipex *pipex, t_list **my_env, int i, t_list **pid_lst)
{
	pid_t	pid;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (!pid)
	{
		close(p_fd[0]);
		if (i)
			dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		exec_cmd(pipex->cmd, my_env, pipex);
	}
	else
	{
		pipex->pid = malloc(sizeof(int) * 1);
		*pipex->pid = pid;
		ft_lstadd_back(pid_lst, ft_lstnew(pipex->pid));
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
	}
}

void	parse_line(t_pipex *pipex, t_list **my_env, t_list **pid_lst)
{
	while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4) != 0)
	{
		if (ft_strncmp(pipex->token->type, "WORD", 4) && ft_open(pipex->token))
		{
			pipex->status = 1;
			while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4) != 0)
			{
				pipex->tmp = pipex->tmp->next;
				if (pipex->tmp)
					pipex->token = pipex->tmp->content;
			}
			return;
		}
		pipex->tmp = pipex->tmp->next;
		if (pipex->tmp)
			pipex->token = pipex->tmp->content;
	}
	if (pipex->cmd && !is_pipe(pipex->token_lst) && is_build_in(pipex->cmd[0]))
		pipex->status = make_build_in(pipex->cmd, my_env, pipex);
	else if (pipex->tmp && pipex->cmd)
		check_cmd(pipex, my_env, 1, pid_lst);
	else if (pipex->cmd)
		check_cmd(pipex, my_env, 0, pid_lst);
	if (pipex->cmd)
		free_tab(pipex->cmd);
}

int	exec_line(t_list *token_lst, t_list **my_env)
{
	t_pipex	pipex;
	t_list	*pid_lst;
	t_list	*tmp;
	int		*i;

	pid_lst = NULL;
	pipex.tmp = token_lst;
	pipex.status = 0;
	pipex.old_stdout = dup(STDOUT_FILENO);
	pipex.old_stdin = dup(STDIN_FILENO);
	pipex.token_lst = token_lst;
	while (pipex.tmp)
	{
		pipex.token = pipex.tmp->content;
		pipex.cmd = get_cmd(pipex.tmp);
		parse_line(&pipex, my_env, &pid_lst);
		if (pipex.tmp && ft_strncmp(pipex.token->type, "PIPE", 4) == 0)
		{
			pipex.status = 0;
			pipex.tmp = pipex.tmp->next;
		}
	}
	tmp = pid_lst;
	while (tmp)
	{
		i = tmp->content;
		waitpid(*i, &pipex.status, 0);
		tmp = tmp->next;
		pipex.status = pipex.status >> 8;
	}
	ft_lstclear(&pid_lst, simple_del);
	dup2(pipex.old_stdout, STDOUT_FILENO);
	dup2(pipex.old_stdin, STDIN_FILENO);
	return (pipex.status);
}
