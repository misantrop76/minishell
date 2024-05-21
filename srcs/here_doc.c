/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:01:01 by ehay              #+#    #+#             */
/*   Updated: 2024/05/21 01:55:20 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_struct_here_doc(t_pipex *pipex, t_list **env, int code, int fd)
{
	int	*fd2;

	if (pipex->cmd)
		free_tab(pipex->cmd);
	ft_lstclear(&pipex->pid_lst, simple_del);
	ft_lstclear(env, simple_del);
	ft_lstclear(pipex->token_lst, del_token);
	close(pipex->old_stdin);
	close(pipex->old_stdout);
	while (pipex->here_doc_p)
	{
		fd2 = pipex->here_doc_p->content;
		close(*fd2);
		pipex->here_doc_p = pipex->here_doc_p->next;
	}
	ft_lstclear(&pipex->here_doc, simple_del);
	rl_clear_history();
	close(fd);
	close(pipex->fd_tmp);
	exit(code);
}

void	handler_ignor(int sig)
{
	(void)sig;
	g_sig_check = 1;
}

void	write_in_pipe(char *limit, int fd, t_pipex *pipex, t_list **env)
{
	char	*buf;

	buf = readline("\001\e[00;96m\002>\001\e[0m\002");
	if (g_sig_check)
		free_struct_here_doc(pipex, env, 130, fd);
	if (!buf)
		free_struct_here_doc(pipex, env, 0, fd);
	if (strncmp(buf, limit, (strlen(buf) + 1)) == 0)
	{
		free(buf);
		free_struct_here_doc(pipex, env, 0, fd);
	}
	if (write(fd, buf, strlen(buf)) == -1)
		free_struct_here_doc(pipex, env, 1, fd);
	write(fd, "\n", 1);
	free(buf);
}

void	ignore_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handler_ignor;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
}

void	heredoc(char *limit, t_pipex *pipex, t_list **env)
{
	pid_t	pid;
	int		p_fd[2];
	int		*new_fd;

	if (pipe(p_fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		close(p_fd[0]);
		while (1)
			write_in_pipe(limit, p_fd[1], pipex, env);
	}
	else
	{
		ignore_signal();
		close(p_fd[1]);
		new_fd = malloc(sizeof(int));
		wait_child(pid);
		*new_fd = dup(p_fd[0]);
		ft_lstadd_back(&pipex->here_doc, ft_lstnew(new_fd));
		close(p_fd[0]);
	}
}
