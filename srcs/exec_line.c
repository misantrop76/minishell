/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_cmd(char **cmd, t_list **env, t_pipex *pipex)
{
	char	*newpath;
	char	**path;
	char	*tmp;
	int		i;

	newpath = NULL;
	i = 0;
	check_make_build_in(pipex, env);
	path = NULL;
	if (get_var("PATH", *env))
		path = ft_split(get_var("PATH", *env), ':');
	free_struct(pipex, env);
	execve(cmd[0], cmd, pipex->env);
	while (path && path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		newpath = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		execve(newpath, cmd, pipex->env);
		free(newpath);
		i++;
	}
	unknown_command(pipex, path);
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
		if (i && pipex->out == 0)
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
		is_stdin(pipex, p_fd[0], i);
		close(p_fd[0]);
	}
}

void	is_cmd(t_pipex *pipex, t_list **my_env, t_list **pid_lst)
{
	if (g_sig_check)
		return ;
	if (pipex->cmd && !is_pipe(*pipex->token_lst) && is_build_in(pipex->cmd[0]))
		pipex->status = make_build_in(pipex->cmd, my_env, pipex);
	else if (pipex->tmp && pipex->cmd)
		check_cmd(pipex, my_env, 1, pid_lst);
	else if (pipex->cmd)
		check_cmd(pipex, my_env, 0, pid_lst);
}

void	parse_line(t_pipex *pipex, t_list **my_env, t_list **pid_lst)
{
	pipex->out = 0;
	pipex->token = pipex->tmp->content;
	pipex->cmd = get_cmd(pipex->tmp);
	while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4) != 0)
	{
		if (ft_strncmp(pipex->token->type, "WORD", 4) && g_sig_check == 0
			&& ft_open(pipex->token, pipex))
		{
			pipex->status = 1;
			while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4) != 0)
			{
				pipex->tmp = pipex->tmp->next;
				if (pipex->tmp)
					pipex->token = pipex->tmp->content;
			}
			return ;
		}
		pipex->tmp = pipex->tmp->next;
		if (pipex->tmp)
			pipex->token = pipex->tmp->content;
	}
	is_cmd(pipex, my_env, pid_lst);
}

int	exec_line(t_list **token_lst, t_list **my_env)
{
	t_pipex	pipex;

	init_pipex(&pipex, my_env, token_lst);
	while (pipex.tmp)
	{
		parse_line(&pipex, my_env, &pipex.pid_lst);
		dup2(pipex.old_stdout, STDOUT_FILENO);
		if (pipex.tmp && ft_strncmp(pipex.token->type, "PIPE", 4) == 0)
		{
			pipex.status = 0;
			pipex.tmp = pipex.tmp->next;
		}
		if (pipex.cmd)
			free_tab(pipex.cmd);
	}
	my_close(&pipex);
	return (pipex.status);
}
