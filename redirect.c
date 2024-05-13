/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:19:09 by ehay              #+#    #+#             */
/*   Updated: 2024/05/13 13:44:25 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > out
int	redirection_out(char *output)
{
	int	fd_output;

	fd_output = open(output, O_CREAT | O_WRONLY | O_TRUNC, 00664);
	if (fd_output == -1)
	{
		ft_putstr_fd("permission denied : ", 1);
		ft_putstr_fd(output, 1);
		ft_putstr_fd("\n", 1);
		return (1);
	}
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		return (1);
	close(fd_output);
	return (0);
}

// >> out a
int	redirection_out_append(char *output)
{
	int	fd_output;

	fd_output = open(output, O_CREAT | O_RDWR | O_APPEND, 00664);
	if (fd_output == -1)
	{
		ft_putstr_fd("permission denied : ", 1);
		ft_putstr_fd(output, 1);
		ft_putstr_fd("\n", 1);
		return (1);
	}
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		return (1);
	close(fd_output);
	return (0);
}

// < input
int	redirection_input(char *input)
{
	int	fd_input;

	if (access(input, R_OK) == 0)
		fd_input = open(input, O_RDONLY, 00664);
	else
	{
		ft_putstr_fd("permission denied : ", 2);
		ft_putstr_fd(input, 2);
		ft_putstr_fd("\n", 2);
		fd_input = open("/dev/null", O_RDONLY, 00664);
		dup2(fd_input, STDIN_FILENO);
		return (1);
	}
	if (dup2(fd_input, STDIN_FILENO) == -1)
		return (1);
	close(fd_input);
	return (0);
}

// << heredoc
int	heredoc(char *limit)
{
	pid_t	pid;
	int		p_fd[2];
	char	*buf;

	if (pipe(p_fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		close(p_fd[0]);
		while (1)
		{
			buf = readline("\001\e[00;96m\002>\001\e[0m\002");
			if (!buf)
				exit(0);
			if (strncmp(buf, limit, (strlen(buf) + 1)) == 0)
			{
				free(buf);
				exit(0);
			}
			if (write(p_fd[1], buf, strlen(buf)) == -1)
				exit(1);
			write(p_fd[1], "\n", 1);
			free(buf);
		}
		close(p_fd[1]);
		exit(0);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		wait(NULL);
	}
	return (0);
}

int	ft_open(t_token *token)
{
	if (ft_strncmp(token->type, "STDOUT_A", 8) == 0)
		return (redirection_out_append(token->value));
	else if (ft_strncmp(token->type, "STDOUT", 6) == 0)
		return (redirection_out(token->value));
	else if (ft_strncmp(token->type, "STDIN", 5) == 0)
		return (redirection_input(token->value));
	else if (ft_strncmp(token->type, "READ", 4) == 0)
		return (heredoc(token->value));
	return (0);
}
