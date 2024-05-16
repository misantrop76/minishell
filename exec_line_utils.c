/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:27:33 by mminet            #+#    #+#             */
/*   Updated: 2024/05/16 18:30:14 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	check_make_build_in(t_pipex *pipex, t_list **env)
{
	int	status;

	close(pipex->old_stdin);
	close(pipex->old_stdout);
	pipex->env = make_env_char(*env);
	if (is_build_in(pipex->cmd[0]))
	{
		status = make_build_in(pipex->cmd, env, pipex);
		free_struct(pipex, env);
		exit(status);
	}
}
