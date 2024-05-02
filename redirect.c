/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehay <ehay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:21 by ehay              #+#    #+#             */
/*   Updated: 2024/05/02 16:36:04 by ehay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > out
void redirection_out(char *input, char *output)
{
	int fd_output;

	if (access(input, R_OK) == 0)
		fd_output = open(output, O_CREAT | O_RDWR | O_APPEND);
	else
	{
		// error(1, input);
		fd_output = open("/dev/null", O_RDWR);
	}
	int fd_output = open(output, O_CREAT | O_RDWR | O_TRUNC);
	if (fd_output == -1)
		// error
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		// error
	close(fd_output);
}


// >> out a
void redirection_out_append(char *input, char *output)
{
	int fd_output;

	if (access(input, R_OK) == 0)
		fd_output = open(output, O_CREAT | O_RDWR | O_APPEND);
	else
	{
		// error(1, input);
		fd_output = open("/dev/null", O_RDWR);
	}
	if (fd_output == -1)
		// error
	if (dup2(fd_output, STDOUT_FILENO) == -1)
		// error
	close(fd_output);
}


// < input
void redirection_input(char *output, char *input)
{
	int fd_input;

    if (access(input, R_OK) == 0)
        fd_input = open(input, O_RDONLY);
    else {
		// error
        fd_input = open("/dev/null", O_RDONLY);
    }
    if (fd_input == -1)
		// error
    if (dup2(fd_input, STDIN_FILENO) == -1)
		// error
    close(fd_input);
}


// << heredoc
void	heredoc(char *limit)
{
	if (charactuel == limit)
	{
		// fermer fini heredoc en redirigant vers la sortie
	}
}
