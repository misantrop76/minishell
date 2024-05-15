/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/05/15 03:09:36 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig_check;

void	exec_cmd(char **cmd, t_list **env, t_pipex *pipex)
{
	char	*newpath;
	char	**path;
	char	**my_env;
	char	*tmp;
	int		i;
	int		status;

	newpath = NULL;
	i = 0;
	close(pipex->old_stdin);
	close(pipex->old_stdout);
	my_env = make_env_char(*env);
	if (is_build_in(cmd[0]))
	{
		status = make_build_in(cmd, env, pipex);
		free_struct(pipex, env, my_env);
		exit(status);
	}
	if (get_var("PATH", *env))
		path = ft_split(get_var("PATH", *env), ':');
	else
		path = ft_split("/bin:/sbin", ':');
	execve(cmd[0], cmd, my_env);
	while (path[i])
	{
		tmp = ft_strjoin(path[i++], "/");
		newpath = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		execve(newpath, cmd, my_env);
		free(newpath);
	}
	unknown_command(pipex, path, env, my_env);
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
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
	}
}

void	parse_line(t_pipex *pipex, t_list **my_env, t_list **pid_lst)
{
	while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4) != 0)
	{
		if (ft_strncmp(pipex->token->type, "WORD", 4) && ft_open(pipex->token,
				pipex, my_env))
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
	if (g_sig_check)
		return;
	if (pipex->cmd && !is_pipe(*pipex->token_lst) && is_build_in(pipex->cmd[0]))
		pipex->status = make_build_in(pipex->cmd, my_env, pipex);
	else if (pipex->tmp && pipex->cmd)
		check_cmd(pipex, my_env, 1, pid_lst);
	else if (pipex->cmd)
		check_cmd(pipex, my_env, 0, pid_lst);
}

void	my_close(t_pipex *pipex)
{
	t_list	*tmp;
	int		*i;
	int		s;

	tmp = pipex->pid_lst;
	while (tmp)
	{
		i = tmp->content;
		waitpid(*i, &s, 0);
		tmp = tmp->next;
		if (pipex->status == 0)
			pipex->status = s >> 8;
	}
	if (g_sig_check)
	{
		g_sig_check = 0;
		pipex->status = 130;
	}
	ft_lstclear(&pipex->pid_lst, simple_del);
	dup2(pipex->old_stdin, STDIN_FILENO);
	dup2(pipex->old_stdout, STDOUT_FILENO);
	close(pipex->old_stdin);
	close(pipex->old_stdout);
}

int	exec_line(t_list **token_lst, t_list **my_env)
{
	t_pipex	pipex;

	pipex.pid_lst = NULL;
	pipex.cmd = NULL;
	pipex.tmp = *token_lst;
	pipex.status = 0;
	pipex.old_stdin = dup(STDIN_FILENO);
	pipex.old_stdout = dup(STDOUT_FILENO);
	pipex.token_lst = token_lst;
	while (pipex.tmp)
	{
		pipex.out = 0;
		pipex.token = pipex.tmp->content;
		pipex.cmd = get_cmd(pipex.tmp);
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
