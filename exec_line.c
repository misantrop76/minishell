/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/05/03 18:58:17 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(char **cmd, char **env, t_list **my_env)
{
	/*int		i;
	char	*path;
	char	*newpath;

	i = 0;*/
	(void)env;
	is_build_in(cmd, my_env);
	// path = get_var(char *cmp, t_list *env);
	// newpath = ft_strjoin(path, list_command[i]);
	// create a new path with origin path + command
	/*if (execve(command_name, args, env) != -1)
	{
		i++;
		return (1);// good this cmd are real
	}*/
	exit (0);
}

int		check_cmd(char **cmd, char **env, t_list **my_env, int i)
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
		exec_cmd(cmd, env, my_env);
	}
	else
	{
		last_pid = pid;
		close(p_fd[1]);
		if (i)
			dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
	}
	return (last_pid);
}

int	exec_line(t_list *token_lst, t_list **my_env, char **env)
{
	char	**cmd;
	t_list	*tmp;
	t_token	*token;
	int		status;
	int		last_pid;
	int		old_stdout;
	int		old_stdin;

	tmp = token_lst;
	old_stdout = dup(STDOUT_FILENO);
	old_stdin = dup(STDIN_FILENO);
	while (tmp)
	{
		token = tmp->content;
		cmd = get_cmd(token_lst);
		while (tmp && ft_strncmp(token->type, "PIPE", 4))
		{
			token = tmp->content;
			if (ft_strncmp(token->type, "WORD", 4))
				ft_open(token);
			tmp = tmp->next;
		}
		if (tmp && cmd)
		{
			last_pid = check_cmd(cmd, env, my_env, 1);
			tmp = tmp->next;
		}
		else if(cmd)
			last_pid = check_cmd(cmd, env, my_env, 0);
		dup2(old_stdout, STDOUT_FILENO);
	}
	if (cmd)
		waitpid(last_pid, &status, 0);
	dup2(old_stdout, STDOUT_FILENO);
	dup2(old_stdin, STDIN_FILENO);
	return (status >> 8);
}
