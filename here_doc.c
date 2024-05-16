/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehay <ehay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:01:01 by ehay              #+#    #+#             */
/*   Updated: 2024/05/16 13:51:04 by ehay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig_check;

void	free_struct_here_doc(t_pipex *pipex, t_list **env, int code)
{
	if (pipex->cmd)
		free_tab(pipex->cmd);
	ft_lstclear(&pipex->pid_lst, simple_del);
	ft_lstclear(env, simple_del);
	ft_lstclear(pipex->token_lst, del_token);
	rl_clear_history();
	printf("exit\n");
	exit(code);
}

void	handler_exit(int sig)
{
	(void)sig;
	g_sig_check = 1;
	ft_putstr_fd("\n", 1);
	dup2(open("/dev/null", O_RDONLY, 00664), STDIN_FILENO);
}

void	write_in_pipe(char *limit, int fd, t_pipex *pipex, t_list **env)
{
	char				*buf;
	struct sigaction	sa;

	sa.sa_handler = &handler_exit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	buf = readline("\001\e[00;96m\002>\001\e[0m\002");
	printf("sig = %d\n", g_sig_check);
	if (g_sig_check)
		free_struct_here_doc(pipex, env, 130);
	if (!buf)
		free_struct_here_doc(pipex, env, 0);
	if (strncmp(buf, limit, (strlen(buf) + 1)) == 0)
	{
		free(buf);
		free_struct_here_doc(pipex, env, 0);
	}
	if (write(fd, buf, strlen(buf)) == -1)
		free_struct_here_doc(pipex, env, 1);
	write(fd, "\n", 1);
	free(buf);
}

int	heredoc(char *limit, t_pipex *pipex, t_list **env)
{
	pid_t	pid;
	int		p_fd[2];

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
		close(p_fd[1]);
		exit(0);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (0);
}
