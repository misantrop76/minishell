/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/05/07 00:53:14 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(char **cmd, t_list **my_env)
{
	char	**path;
	char	*test;
	char	**args;
	int		i;

	is_build_in(cmd, my_env);
	path = ft_split(get_var("PATH", *my_env), ':');
	args = cmd;
	i = 0;
	execve(args[0], args, NULL);
	while (path[i])
	{
		test = ft_strjoin(ft_strjoin(path[i], "/"), args[0]);
		execve(test, args, NULL);
		free(test);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd("\n", 2);
	free_tab(path);
	free_tab(args);
	exit(127);
	// char	*path;
	// char	*newpath;
	/*int		i;
	i = 0;*/
	// path = get_var(char *cmp, t_list *env);
	// newpath = ft_strjoin(path, list_command[i]);
	// create a new path with origin path + command
	/*if (execve(command_name, args, env) != -1)
	{
		i++;
		return (1);// good this cmd are real
	}*/
}

int	check_cmd(char **cmd, t_list **my_env, int i)
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
		exec_cmd(cmd, my_env);
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
	pipex->token = pipex->tmp->content;
	pipex->cmd = get_cmd(pipex->tmp);
	if (pipex->cmd)
		printf("commande :%s token value = %s\n", pipex->cmd[0], pipex->token->value);
	while (pipex->tmp && ft_strncmp(pipex->token->type, "PIPE", 4))
	{
		if (ft_strncmp(pipex->token->type, "WORD", 4))
			ft_open(pipex->token);
		pipex->tmp = pipex->tmp->next;
		if (pipex->tmp)
			pipex->token = pipex->tmp->content;
	}
	if (pipex->tmp && pipex->cmd && ft_strncmp(pipex->cmd[0], "exit", 6))
		pipex->last_pid = check_cmd(pipex->cmd, my_env, 1);
	else if (pipex->cmd && ft_strncmp(pipex->cmd[0], "exit", 6))
		pipex->last_pid = check_cmd(pipex->cmd, my_env, 0);
	else if (pipex->cmd && ft_strncmp(pipex->cmd[0], "exit", 6) == 0)
		make_exit(pipex, my_env);
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
	while (pipex.tmp)
	{
		parse_line(&pipex, my_env);
		if (pipex.tmp && ft_strncmp(pipex.token->type, "PIPE", 4) == 0)
			pipex.tmp = pipex.tmp->next;
	}
	//if (pipex.cmd)
		waitpid(pipex.last_pid, &pipex.status, 0);
	dup2(pipex.old_stdout, STDOUT_FILENO);
	dup2(pipex.old_stdin, STDIN_FILENO);
	return (pipex.status >> 8);
}
