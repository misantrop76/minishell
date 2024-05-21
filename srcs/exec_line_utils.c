/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:27:33 by mminet            #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_stdin(t_pipex *pipex, int fd, int i)
{
	t_list	*tmp;
	t_token	*token;

	tmp = pipex->tmp;
	while (tmp && i)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "STDIN", 5) == 0 || ft_strncmp(token->type,
				"READ", 4) == 0)
		{
			dup2(fd, pipex->fd_tmp);
			return ;
		}
		tmp = tmp->next;
	}
	//if (i)
		dup2(fd, STDIN_FILENO);
}

void	init_pipex(t_pipex *pipex, t_list **env, t_list **token_lst)
{
	pipex->pid_lst = NULL;
	pipex->cmd = NULL;
	pipex->here_doc = NULL;
	pipex->tmp = *token_lst;
	pipex->status = 0;
	pipex->old_stdin = dup(STDIN_FILENO);
	pipex->old_stdout = dup(STDOUT_FILENO);
	pipex->token_lst = token_lst;
	pipex->fd_tmp = open("/dev/null", O_CREAT | O_RDWR | O_APPEND, 00664);
	pipex->env = NULL;
	fill_here_doc(pipex, env);
	pipex->here_doc_p = pipex->here_doc;
}

void	clear(t_pipex *pipex)
{
	ft_lstclear(&pipex->pid_lst, simple_del);
	ft_lstclear(&pipex->here_doc, simple_del);
	dup2(pipex->old_stdin, STDIN_FILENO);
	dup2(pipex->old_stdout, STDOUT_FILENO);
	close(pipex->old_stdin);
	close(pipex->old_stdout);
	close(pipex->fd_tmp);
}

void	my_close(t_pipex *pipex)
{
	t_list	*tmp;
	int		*i;
	int		s;
	int		status_open;

	tmp = pipex->pid_lst;
	status_open = pipex->status;
	while (tmp)
	{
		i = tmp->content;
		if (waitpid(*i, &s, 0) != -1 && status_open == 0)
			pipex->status = s >> 8;
		tmp = tmp->next;
	}
	if (g_sig_check)
	{
		g_sig_check = 0;
		pipex->status = 130;
	}
	clear(pipex);
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
		free_tab(pipex->cmd);
		free_tab(pipex->env);
		exit(status);
	}
}
