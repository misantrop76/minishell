/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehay <ehay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 19:57:55 by mminet            #+#    #+#             */
/*   Updated: 2024/05/16 14:27:24 by ehay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_struct(t_pipex *pipex, t_list **env, char **my_env)
{
	(void)pipex;
	(void)env;
	free_tab(pipex->cmd);
	free_tab(my_env);
	ft_lstclear(&pipex->pid_lst, simple_del);
	ft_lstclear(env, simple_del);
	ft_lstclear(pipex->token_lst, del_token);
	rl_clear_history();
}

int	ft_putstr_fd_return_status(char *str, int fd, int status)
{
	if (str)
		write(fd, str, ft_strlen(str));
	return (status);
}

void	unknown_command(t_pipex *pipex, char **path, t_list **env,
		char **my_env)
{
	int			status;
	struct stat	buf;
	int			i;

	status = 127;
	i = 0;
	ft_putstr_fd(pipex->cmd[0], 1);
	while (pipex->cmd[0][i] && pipex->cmd[0][i] != '/')
		i++;
	if (pipex->cmd[0][i] && stat(pipex->cmd[0], &buf) == 0
		&& S_ISDIR(buf.st_mode))
		status = ft_putstr_fd_return_status(": Is a directory\n", 2, 126);
	else if (!pipex->cmd[0][i])
		ft_putstr_fd(" : command not found\n", 2);
	else if (access(pipex->cmd[0], X_OK) != 0 && errno == EACCES)
		status = ft_putstr_fd_return_status(": Permission denied\n", 2, 126);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
	free_struct(pipex, env, my_env);
	free_tab(path);
	exit(status);
}

	// {
	// 	ft_putstr_fd(": Is a directory\n", 2);
	// 	status = 126;
	// }

	// {
	// 	status = 126;
	// 	ft_putstr_fd(": Permission denied\n", 2);
	// }

char	**make_env_char(t_list *env)
{
	char	**new_env;
	t_list	*tmp;
	int		i;

	new_env = malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	i = 0;
	tmp = env;
	while (tmp)
	{
		new_env[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	is_pipe(t_list *token_lst)
{
	t_list	*tmp;
	t_token	*token;

	tmp = token_lst;
	while (tmp)
	{
		token = tmp->content;
		if (ft_strncmp(token->type, "PIPE", 4) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
