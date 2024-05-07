/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehay <ehay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:19:09 by ehay              #+#    #+#             */
/*   Updated: 2024/05/07 13:19:10 by ehay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > out
void redirection_out(char *output)
{
	int fd_output;

	fd_output = open(output, O_CREAT | O_RDWR | O_TRUNC, 00664);
	// if (fd_output == -1)
	// 	// error
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		// error
	close(fd_output);
}


// >> out a
void redirection_out_append(char *output)
{
	int fd_output;

	fd_output = open(output, O_CREAT | O_RDWR | O_APPEND, 00664);
	// if (fd_output == -1)
	// 	// error
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		// error
	close(fd_output);
}


// < input
void redirection_input(char *input)
{
	int fd_input;

    if (access(input, R_OK) == 0)
        fd_input = open(input, O_RDONLY, 00664);
    else
	{
		// error
        fd_input = open("/dev/null", O_RDONLY, 00664);
    }
    if (dup2(fd_input, STDIN_FILENO) == -1)
		// error
    close(fd_input);
}


// << heredoc
void	heredoc(char *limit)
{
	pid_t	pid;
	int 	p_fd[2];
	char	*buf;

	if (pipe(p_fd) == -1)
	{
		// error
		printf("error 1");
		exit(0);
	}
	pid = fork();
	if (pid == -1)
	{
		// error
		printf("error la");
		exit(0);
	}
	if (pid == 0)
	{
		close(p_fd[0]);
		while (1)
        {
            buf = readline("\001\e[00;96m\002>\001\e[0m\002");
            if (!buf)
                break;
            if (strncmp(buf, limit, (strlen(buf) + 1)) == 0)
            {
                free(buf);
                break;
            }
            if (write(p_fd[1], buf, strlen(buf) + 1) == -1)
                exit(1);
			write(p_fd[1], "\n", 1);
            free(buf);
        }
		close(p_fd[1]);
		exit(1);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		wait(NULL);
	}
}

void	ft_open(t_token *token)
{
	if (ft_strncmp(token->type, "STDOUT_A", 8) == 0)
		redirection_out_append(token->type);
	else if (ft_strncmp(token->type, "STDOUT", 6) == 0)
		redirection_out(token->value);
	else if (ft_strncmp(token->type, "STDIN", 5) == 0)
		redirection_input(token->value);
	else if (ft_strncmp(token->type, "READ", 4) == 0)
		heredoc(token->value);
}
