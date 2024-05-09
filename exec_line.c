/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/05/09 04:03:40 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_pipe(t_list *token_lst)
{
	t_list *tmp;
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
	char	*tmp;
	int		i;

	newpath = NULL;
	i = 0;
	if (is_build_in(cmd[0]))
		exit(make_build_in(cmd, env, pipex));
	path = ft_split(get_var("PATH", *env), ':');
	execve(cmd[0], cmd, NULL);
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		newpath = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		execve(newpath, cmd, NULL);
		free(newpath);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd("\n", 2);
	free_tab(path);
	exit(127);
}

int	check_cmd(t_pipex *pipex, t_list **my_env, int i)
{
	pid_t	pid;
	int		p_fd[2];
	int		last_pid;

	if (pipe(p_fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
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
		last_pid = pid;
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
	}
	return (last_pid);
}

void	parse_line(t_pipex *pipex, t_list **my_env)
{
	while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4))
	{
		if (ft_strncmp(pipex->token->type, "WORD", 4))
			ft_open(pipex->token);
		pipex->tmp = pipex->tmp->next;
		if (pipex->tmp)
			pipex->token = pipex->tmp->content;
	}
	if (pipex->cmd  && !is_pipe(pipex->token_lst) && is_build_in(pipex->cmd[0]))
		pipex->status = make_build_in(pipex->cmd, my_env, pipex);
	else if (pipex->tmp && pipex->cmd)
		pipex->last_pid = check_cmd(pipex, my_env, 1);
	else if (pipex->cmd)
		pipex->last_pid = check_cmd(pipex, my_env, 0);
	if (pipex->cmd)
		free_tab(pipex->cmd);
}

int	exec_line(t_list *token_lst, t_list **my_env)
{
	t_pipex	pipex;

	pipex.tmp = token_lst;
	pipex.old_stdout = dup(STDOUT_FILENO);
	pipex.old_stdin = dup(STDIN_FILENO);
	pipex.token_lst = token_lst;
	pipex.last_pid = 0;
	while (pipex.tmp)
	{
		pipex.token = pipex.tmp->content;
		pipex.cmd = get_cmd(pipex.tmp);
		parse_line(&pipex, my_env);
		if (pipex.tmp && ft_strncmp(pipex.token->type, "PIPE", 4) == 0)
			pipex.tmp = pipex.tmp->next;
	}
	if (pipex.last_pid)
		waitpid(pipex.last_pid, &pipex.status, 0);
	dup2(pipex.old_stdout, STDOUT_FILENO);
	dup2(pipex.old_stdin, STDIN_FILENO);
	return (pipex.status >> 8);
}
