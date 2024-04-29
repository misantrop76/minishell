/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:59:41 by ehay              #+#    #+#             */
/*   Updated: 2024/04/29 13:30:52 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_cmd_env()
{
	int i;
	char *path;
	char *newpath;

	i = 0;
	// path = get_var(char *cmp, t_list *env);
	newpath = NULL;
	while ()
	{
		newpath = strjoin(path, list_command[i]); // create a new path with origin path + command
		if (execve(command_name, args, env) != -1)
		{
			i++;
			return (1) // good this cmd are real
		}
	}
	return (0); // no cmd find
}


int			exec_line(t_token *token_lst, t_list *my_env, char **env)
{
	char	**cmd;
	t_list	*tmp;
	t_token *token;
	int status;

	tmp = token_lst;
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
		if (tmp)
			//exec_with_pipe(cmd, my_env, env);
		else
			//last_exec(cmd, my_env, env);
		tmp = tmp->next;
	}
	waitpid(last_pid, &status);
	return (status);
}
