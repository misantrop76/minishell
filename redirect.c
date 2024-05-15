/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:19:09 by ehay              #+#    #+#             */
/*   Updated: 2024/05/15 00:47:15 by mminet           ###   ########.fr       */
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
		ft_putstr_fd(output, 2);
		ft_putstr_fd(": Permission non accordée\n", 2);
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
		ft_putstr_fd(output, 2);
		ft_putstr_fd(": Permission non accordée\n", 2);
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
		ft_putstr_fd(input, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission non accordée\n", 2);
		else
			ft_putstr_fd(": Aucun fichier ou dossier de ce nom\n", 2);
		fd_input = open("/dev/null", O_RDONLY, 00664);
		dup2(fd_input, STDIN_FILENO);
		return (1);
	}
	if (dup2(fd_input, STDIN_FILENO) == -1)
		return (1);
	close(fd_input);
	return (0);
}

int	ft_open(t_token *token, t_pipex *pipex)
{
	if (ft_strncmp(token->type, "STDIN", 5) == 0 || ft_strncmp(token->type,
			"READ", 4) == 0)
		dup2(pipex->old_stdin, STDIN_FILENO);
	if (ft_strncmp(token->type, "STDOUT", 6) == 0)
		pipex->out = 1;
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
