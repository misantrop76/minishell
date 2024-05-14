#include "minishell.h"

void	write_in_pipe(char *limit, int fd)
{
	char	*buf;

	buf = readline("\001\e[00;96m\002>\001\e[0m\002");
	if (!buf)
		exit(0);
	if (strncmp(buf, limit, (strlen(buf) + 1)) == 0)
	{
		free(buf);
		exit(0);
	}
	if (write(fd, buf, strlen(buf)) == -1)
		exit(1);
	write(fd, "\n", 1);
	free(buf);
}

int	heredoc(char *limit)
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
			write_in_pipe(limit, p_fd[1]);
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
